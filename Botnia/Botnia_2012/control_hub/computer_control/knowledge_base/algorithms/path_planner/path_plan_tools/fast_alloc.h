/*========================================================================
    FastAlloc.h :  A pool-based memory allocator for same-sized objects
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

#ifndef __FAST_ALLOC_H__
#define __FAST_ALLOC_H__

#define FSTALC_TEM template <class item_t>
#define FSTALC_FUN fast_allocator<item_t>

// #define DEBUG 1
//����һ�����нڵ��б�
FSTALC_TEM
class fast_allocator
{
	item_t *free_list;
	int num_alloc,num_free;
public:
	fast_allocator()
	{
		free_list=NULL;
		num_alloc=num_free=0;
	}
	~fast_allocator();

	item_t *alloc();
	void free(item_t *item);
	void freelist(item_t *item);

	int count_allocated()
	{
		return(num_alloc);
	}
	int count_free()
	{
		return(num_free);
	}
};

FSTALC_TEM
FSTALC_FUN::~fast_allocator()
{
	item_t *p,*q;
	p = free_list;
	while (q = p)
	{
		p = p->next;
		q->next = NULL;
		delete(q);
	}
	free_list = NULL;
	num_alloc = num_free = 0;
}

//�ӿ����б���ժ��һ�����
FSTALC_TEM
item_t *FSTALC_FUN::alloc()
{
    //qDebug()<<"num_alloc: "<<num_alloc<<"num_free: "<<num_free;

	item_t *p;
	//������������գ��ӿ���������ժ��һ��
	if (free_list)
	{
		p = free_list;
		free_list = p->next;
		p->next = NULL;
		num_free--;
	}
	//���������ϵͳ����һ���ռ�
	else
	{
		p = new item_t;
	}
	num_alloc++;
	return(p);
}

//�黹һ���ڵ�������б�
FSTALC_TEM
void FSTALC_FUN::free(item_t *item)
{
    //qDebug()<<"num_alloc: "<<num_alloc<<"num_free: "<<num_free;
	//�����յĿռ���õ�����������
	item->next = free_list;
	free_list = item;
	num_free++;
	num_alloc--;
}

//�黹һ�������е����нڵ�������б�
FSTALC_TEM
void FSTALC_FUN::freelist(item_t *item)
{
    //qDebug()<<"num_alloc: "<<num_alloc<<"num_free: "<<num_free;
	item_t *n;
	while (item)
	{
		n = item->next;
		item->next = free_list;
		free_list = item;
		item = n;
		num_free++;
		num_alloc--;
	}
}

#endif /*__FAST_ALLOC_H__*/
