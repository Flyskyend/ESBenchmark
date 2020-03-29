#include <iostream>
//#include <sys/time.h>
//#include <unistd.h>
#include <vector>
#include <cmath>
#include "trace.h"
#include "loser_tree.h"

using namespace std;

LoserTree::LoserTree(int fn, vector<vector<trace>> *ft)
{
	cout << "Creating New LoserTree...  ";
	filetop = ft;
	filenum = fn;
	loser_tree.reserve(2 * fn);
	CreateLoserTree();
	//PrintTreeNodes();
	cout << "Completed." << endl;
}

void LoserTree::CreateLoserTree()
{
	int lastlbegin = 0;
	int listid = 0;
	for (int leafnum = 1; leafnum <= filenum; level++, leafnum *= 2)
	{
		//printf("filenum=%d,leafnum=%d,level=%d\n",filenum,leafnum,level);
		if (leafnum == 1)
		{
			struct loser_tree_node tmp;
			tmp.fileid = -1;
			tmp.listid = listid++;
			loser_tree.push_back(tmp);
			tmp.parent = NULL;
			root = &loser_tree.back();
		}
		else
		{
			lastlbegin = loser_tree.size() - pow(2, level - 1);
			//printf("lastlbegin=%d, loser_tree.size=%d, pow(level)=%d\n",lastlbegin,loser_tree.size(),int(pow(2,level-1)));
			for (int j = 0; j < leafnum; j += 2)
			{
				//printf("j=%d, leafnume=%d, lastlbegin+j/2=%d\n",j,leafnum,lastlbegin+j/2);
				struct loser_tree_node tmp;
				tmp.fileid = -1;
				tmp.listid = listid++;
				tmp.parent = &loser_tree[lastlbegin + j / 2];
				//printf("tmp.parent->%d\n",lastlbegin+j/2);
				loser_tree.push_back(tmp);
				loser_tree[lastlbegin + j / 2].left = &loser_tree[loser_tree.size() - 1];
				//printf("loser_tree[%d].left->%d:%d\n",lastlbegin+j/2,loser_tree.size()-1,loser_tree[lastlbegin+j/2].left->listid);
				tmp.listid = listid++;
				loser_tree.push_back(tmp);
				loser_tree[lastlbegin + j / 2].right = &loser_tree[loser_tree.size() - 1];
				//printf("loser_tree[%d].right->%d:%d %x\n",lastlbegin+j/2,loser_tree.size()-1,loser_tree[lastlbegin+j/2].right->listid,&loser_tree[loser_tree.size()-1]);
			}

		}

		lastlbegin = loser_tree.size() - pow(2, level);
		if (leafnum * 2 > filenum)
		{
			int newlevelnum = filenum - leafnum;
			int i;
			int fileid = 0;
			for (i = 0; newlevelnum > 0; newlevelnum--, i += 2)
			{
				//printf("i=%d, newlevelnum=%d, lastlbegin+i/2=%d, size=%d\n",i,newlevelnum,lastlbegin+i/2,loser_tree.size());
				struct loser_tree_node tmp;
				tmp.fileid = fileid++;
				tmp.listid = listid++;
				tmp.left = NULL;
				tmp.right = NULL;
				tmp.parent = &loser_tree[lastlbegin + i / 2];
				loser_tree.push_back(tmp);
				loser_tree[lastlbegin + i / 2].left = &loser_tree.back();
				tmp.fileid = fileid++;
				tmp.listid = listid++;
				loser_tree.push_back(tmp);
				loser_tree[lastlbegin + i / 2].right = &loser_tree.back();
			}
			i /= 2;
			for (int j = leafnum * 2 - filenum; j > 0; i++, j--)
			{
				//printf("j=%d, i=%d, lastlbegin+i=%d, size=%d",j,i,lastlbegin+i,loser_tree.size());
				loser_tree[lastlbegin + i].fileid = fileid++;
				loser_tree[lastlbegin + i].left = NULL;
				loser_tree[lastlbegin + i].right = NULL;
				//printf(", fileid=%d\n",loser_tree[lastlbegin+i].fileid);
			}
			//printf("node%d: fileid=%d, listid=%d , %x\n",lastlbegin+i-1,loser_tree[lastlbegin+i-1].fileid,loser_tree[lastlbegin+i-1].listid,&loser_tree[lastlbegin+i-1]);
			//printf("node14: rfileid=%d, rlistid=%d, %x\n",loser_tree[14].right->fileid,loser_tree[14].right->listid,loser_tree[14].right);
		}
	}

	//PrintTreeNodes();

	for (int i = loser_tree.size() - 1; i >= 0; i--)
	{
		if (loser_tree[i].fileid < 0)
		{
			NodeCompare(i);
		}
		else
		{
			loser_tree[i].loser = -1;
			loser_tree[i].winner = -1;
		}

	}
}

void LoserTree::NodeCompare(int listid)
{
	double left, right;
	int leftid, rightid;
	//cout<<listid<<endl;
	//printf("listid=%d, lfileid=%d, rfileid=%d, llistid=%d, rlistid=%d\n",listid,loser_tree[listid].left->fileid,loser_tree[listid].right->fileid,loser_tree[listid].left->listid,loser_tree[listid].right->listid);
	if (loser_tree[listid].left->fileid < 0)
	{
		leftid = loser_tree[listid].left->winner;
		if (leftid != -1)
			left = filetop->at(loser_tree[leftid].fileid).front().key[TRACE_SORT_ITEM_INDEX];
	}
	else
	{
		leftid = loser_tree[listid].left->listid;
		if (!filetop->at(loser_tree[leftid].fileid).empty())
		{
			left = filetop->at(loser_tree[leftid].fileid).front().key[TRACE_SORT_ITEM_INDEX];
		}
		else
			leftid = -1;
	}

	if (loser_tree[listid].right->fileid < 0)
	{
		rightid = loser_tree[listid].right->winner;
		if (rightid != -1)
			right = filetop->at(loser_tree[rightid].fileid).front().key[TRACE_SORT_ITEM_INDEX];
	}
	else
	{
		rightid = loser_tree[listid].right->listid;
		if (!filetop->at(loser_tree[rightid].fileid).empty())
			right = filetop->at(loser_tree[rightid].fileid).front().key[TRACE_SORT_ITEM_INDEX];
		else
			rightid = -1;
	}

	//printf("rightid=%d, leftid=%d\n",rightid,leftid);

	if (leftid == -1 && rightid == -1)
	{
		loser_tree[listid].winner = rightid;
		loser_tree[listid].loser = leftid;
		return;
	}
	else if (leftid == -1)
	{
		loser_tree[listid].winner = rightid;
		loser_tree[listid].loser = leftid;
		return;
	}
	else if (rightid == -1)
	{
		loser_tree[listid].winner = leftid;
		loser_tree[listid].loser = rightid;
		return;
	}

	if (left <= right)
	{
		loser_tree[listid].winner = leftid;
		loser_tree[listid].loser = rightid;
	}
	else
	{
		loser_tree[listid].winner = rightid;
		loser_tree[listid].loser = leftid;
	}
}

void LoserTree::PrintTreeNodes()
{
	for (int i = 0; i < loser_tree.size(); i++)
	{
		cout << &loser_tree[i] << endl;
		//sleep(1);
		// if(loser_tree[i].fileid<0)
		//     printf("node%d:%x fileid=%d parentid=%d, leftid=%d, rightid=%d, loser=%d, winner=%d, lfileid=%d ,rfileid=%d\n",i,&loser_tree[i],loser_tree[i].fileid,loser_tree[i].parent->listid,loser_tree[i].left->listid,loser_tree[i].right->listid,loser_tree[i].loser,loser_tree[i].winner,loser_tree[i].left->fileid,loser_tree[i].right->fileid);
		// else
		// {
		//     printf("node%d:%x fileid=%d parentid=%d, loser=%d, winner=%d\n",i,&loser_tree[i],loser_tree[i].fileid,loser_tree[i].parent->listid,loser_tree[i].loser,loser_tree[i].winner);
		// }
	}
}

void LoserTree::PrintTreeNode(int listid)
{
	if (loser_tree[listid].fileid < 0)
		printf("node%d:%x fileid=%d parentid=%d, leftid=%d, rightid=%d, loser=%d, winner=%d, lfileid=%d ,rfileid=%d\n", listid, &loser_tree[listid], loser_tree[listid].fileid, loser_tree[listid].parent->listid, loser_tree[listid].left->listid, loser_tree[listid].right->listid, loser_tree[listid].loser, loser_tree[listid].winner, loser_tree[listid].left->fileid, loser_tree[listid].right->fileid);
	else
		printf("node%d:%x fileid=%d parentid=%d, loser=%d, winner=%d\n", listid, &loser_tree[listid], loser_tree[listid].fileid, loser_tree[listid].parent->listid, loser_tree[listid].loser, loser_tree[listid].winner);
}

int LoserTree::GetWinner()
{
	int winner = loser_tree[root->winner].fileid;

	//cout<<"Winner File: "<<winner<<endl;
	//PrintTreeNode(winner);
	//filetop->at(winner).pop();
	//UpdateTree(loser_tree[root->winner].parent->listid);

	return winner;
}

void LoserTree::UpdateTree()
{
	int listid = loser_tree[root->winner].parent->listid;
	UpdateTreeNode(listid);
}

void LoserTree::UpdateTreeNode(int listid)
{
	//PrintTreeNode(listid);
	NodeCompare(listid);
	if (loser_tree[listid].listid != 0)
		UpdateTreeNode(loser_tree[listid].parent->listid);
}