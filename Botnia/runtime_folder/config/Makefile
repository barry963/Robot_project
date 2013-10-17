# /* LICENSE:
#   =========================================================================
#     CMDragons'02 RoboCup F180 Source Code Release
#   -------------------------------------------------------------------------
#     Copyright (C) 2002 Manuela Veloso, Brett Browning, Mike Bowling,
#                        James Bruce; {mmv, brettb, mhb, jbruce}@cs.cmu.edu
#     School of Computer Science, Carnegie Mellon University
#   -------------------------------------------------------------------------
#     This software is distributed under the GNU General Public License,
#     version 2.  If you do not have a copy of this licence, visit
#     www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
#     Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
#     in the hope that it will be useful, but WITHOUT ANY WARRANTY,
#     including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#   ------------------------------------------------------------------------- */

TOPDIR=..
include $(TOPDIR)/Makefile.common
SRCDIR=/home/jbruce/robot/small/code01/config.sony

# target files
THRESH = thresh.tmap
CAMERA = camera1.txt
COLOR = colors.txt

# make symlinks
all::
	ln -f -s $(SRCDIR)/$(THRESH) $(THRESH)
	ln -f -s $(SRCDIR)/$(CAMERA) $(CAMERA)
	ln -f -s $(SRCDIR)/$(COLOR)  $(COLOR)
