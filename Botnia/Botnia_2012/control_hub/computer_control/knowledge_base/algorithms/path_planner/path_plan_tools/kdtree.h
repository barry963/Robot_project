/*========================================================================
    KDTree.h : Template for fast online creation KD Trees
  ------------------------------------------------------------------------
    Copyright (C) 1999-2002  James R. Bruce
    School of Computer Science, Carnegie Mellon University
  ------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ========================================================================*/

#ifndef __KD_TREE_H__
#define __KD_TREE_H__

#include "control_hub/computer_control/knowledge_base/algorithms/path_planner/path_plan_tools/fast_alloc.h"
#include "control_hub/computer_control/intelligence/world_analysor/analizing_tools/field_geometry.h"
#include "user_interface/field_related/field_constants.h"

#define KDT_TEMP template <class state>
#define KDT_FUN  KDTree<state>

KDT_TEMP
class KDTree
{
	struct node
	{
		//每个节点都有范围
		vector2f minv,maxv; // bounding box of subtree
		state *states;      // list of states stored at this node
		int num_states;     // number of states of this subtree

		union
		{
			node *child[2]; // children of this tree
			node *next;
		};
	};

	node *root;
	int leaf_size,max_depth;
	int tests;
	//用于快速分配的管理器，原理就是建立空闲列表，保存废弃的空间
	//下次申请，首先从废弃空间分配，不够再向操作系统申请空间
	fast_allocator<node> anode;

protected:
	inline bool inside(vector2f &minv,vector2f &maxv,state &s);
	inline float box_distance(vector2f &minv,vector2f &maxv,vector2f &p);

	void split(node *t,int split_dim);
	state *nearest(node *t,state *best,float &best_dist,vector2f &x);
	void clear(node *t);
public:
	KDTree()
	{
		root=NULL;
		leaf_size=max_depth=0;
	}

	bool setdim(vector2f &minv,vector2f &maxv,int nleaf_size,int nmax_depth);
	bool add(state *s);
	void clear();
	state *nearest(float &dist,vector2f &x);
};

//检查位置s是否在minv和maxv两点限定的范围之内
KDT_TEMP
inline bool KDT_FUN::inside(vector2f &minv,vector2f &maxv,state &s)
{
	return(s.pos.x>minv.x && s.pos.y>minv.y &&
	       s.pos.x<maxv.x && s.pos.y<maxv.y);
}

//计算点p到由minv和maxv确定的矩形的距离
KDT_TEMP
inline float KDT_FUN::box_distance(vector2f &minv,vector2f &maxv,vector2f &p)
{
	float dx,dy;
	dx = p.x - bound(p.x,minv.x,maxv.x);
	dy = p.y - bound(p.y,minv.y,maxv.y);
	return(sqrt(dx*dx + dy*dy));
}

//将制定节点的states按照坐标进行对分
//从分裂算法看节点t必须是叶子节点
KDT_TEMP
void KDT_FUN::split(node *t,int split_dim)
{
	node *a,*b;
	state *p,*n;
	float split_val;
	//生成两个新的节点，并初始化为空
	// make new nodes
    a = anode.alloc();
    b = anode.alloc();
    qDebug()<<"allocate for two child!";
	if (!a || !b) return;
	a->child[0] = b->child[0] = NULL;
	a->child[1] = b->child[1] = NULL;
	a->states = b->states = NULL;
	a->num_states = b->num_states = 0;
	// determine split value
	a->minv = b->minv = t->minv;
	a->maxv = b->maxv = t->maxv;
	//如果split_dim=0，则按照X值进行两分，否则按照Y进行两分
	if (split_dim == 0)
	{
		split_val = (t->minv.x + t->maxv.x) / 2;
		a->maxv.x = b->minv.x = split_val;
	}
	else
	{
		split_val = (t->minv.y + t->maxv.y) / 2;
		a->maxv.y = b->minv.y = split_val;
	}
	// separate children based on split
	//
	n = t->states;//n=节点t的链表表头
	while (p = n)
	{
		n = n->next;
		if (((split_dim == 0)?p->pos.x : p->pos.y) < split_val)
		{
			//将p插入到a的states列表的表头
			p->next = a->states;
			a->states = p;
			//计数器增加
			a->num_states++;
		}
		else
		{
			//将p插入到b的states列表的表头
			p->next = b->states;
			b->states = p;
			//计数器增加
			b->num_states++;
		}
	}
	// insert into tree
	t->states = NULL;
	t->child[0] = a;
	t->child[1] = b;
}

//设定树特性，并为树分配空间
KDT_TEMP
bool KDT_FUN::setdim(vector2f &minv,vector2f &maxv,int nleaf_size,int nmax_depth)
{
	clear();
	if (!root)
	{
		//为根节点申请一个空间
        root = anode.alloc();
	}
    if (!root)
	{
        return false;
	}
	//设定根节点属性
	mzero(*root);
	root->minv = minv;
	root->maxv = maxv;
	leaf_size = nleaf_size;
	max_depth = nmax_depth;
    return true;
}

//将节点增加到树中，
//根据算法，数据总是保存在叶子节点中
KDT_TEMP
bool KDT_FUN::add(state *s)
{
	node *p;
	int c,level;
	level = 0;
	p = root;
	if (!p || !inside(p->minv,p->maxv,*s))
	{
		//如果不存在根节点或者点s不在范围内则返回false
		return(false);
	}
	// go down tree to see where new state should go
	//深度优先搜索，直到找到一个包含此节点分支的叶子节点
	while (p->child[0])
	{ // implies p->child[1] also
		//如果不在左子数范围内，就检查右子树，直到找到一个空子树
		c = !inside(p->child[0]->minv,p->child[0]->maxv,*s);
		p = p->child[c];
		level++;
	}
	// add it to leaf; and split leaf if too many children
	//加入列表
	s->next = p->states;
	p->states = s;
	p->num_states++;
	//由于每次都是对半分解，所以，如果存在child[0]就一定存在child[1]
	// split leaf if not too deep and too many children for one node
	if (level<max_depth && p->num_states>leaf_size)
	{
		//偶数层按照X对半分，奇数层按照Y对半分
		split(p,level % 2);
	}
	return(true);
}

//清除指定节点下的信息
KDT_TEMP
void KDT_FUN::clear(node *t)
{
	if (!t)
	{
		return;
	}
	if (t->child[0])
	{
		clear(t->child[0]);
	}
	if (t->child[1])
	{
		clear(t->child[1]);
	}
	t->child[0] = t->child[1] = NULL;
	t->states = NULL;
	t->num_states = 0;
    anode.free(t);
    t=NULL;
}

//清除整个树
KDT_TEMP
void KDT_FUN::clear()
{
	if (!root)
	{
		return;
	}
    clear(root->child[0]);
    clear(root->child[1]);
    root->child[0] = root->child[1] = NULL;
    root->states = NULL;
    root->num_states = 0;
    qDebug()<<"clear one";
}

//
KDT_TEMP
state *KDT_FUN::nearest(node *t,state *best,float &best_dist,vector2f &x)
{
	float d,dc[2];
	state *p;
	int c;
	// look at states at current node
	//首先在states列表中寻找
	p = t->states;
	while (p)
	{
		d = MyVector::distance(p->pos,x);
		if (d < best_dist)
		{
			best = p;
			best_dist = d;
		}
		tests++;
		p = p->next;
	}
	// recurse on children (nearest first to maximize pruning)
	//有点疑问，为什么只检查一边的子节点
	if (t->child[0])
	{ // implies t->child[1]
		dc[0] = box_distance(t->child[0]->minv,t->child[0]->maxv,x);
		dc[1] = box_distance(t->child[1]->minv,t->child[1]->maxv,x);
		c = dc[1] < dc[0]; // c indicates nearest lower bound distance child
		//首先搜索距离小的分支
		if (dc[ c] < best_dist)
		{
			best = nearest(t->child[ c],best,best_dist,x);
		}
		//再搜索距离大的分支
		if (dc[!c] < best_dist)
		{
			best = nearest(t->child[!c],best,best_dist,x);
		}
	}
	return(best);
}


KDT_TEMP
state *KDT_FUN::nearest(float &dist,vector2f &x)
{
	state *best;
	best = NULL;
	dist = sqrt((double)(
                        FieldConstantsRoboCup2012::field_width_*FieldConstantsRoboCup2012::field_width_+
                        FieldConstantsRoboCup2012::field_length_*FieldConstantsRoboCup2012::field_length_))
	       +100;
	tests = 0;
	best = nearest(root,best,dist,x);
	//显示经过多少次比较
	//printf("tests=%d dist=%f\n\n",tests,dist);
	return(best);
}

#endif /*__KD_TREE_H__*/
