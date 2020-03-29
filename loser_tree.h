#ifndef LOSER_TREE_
#define LOSER_TREE_

#include <vector>
#include "trace.h"

using namespace std;

struct loser_tree_node
{
	int fileid;
	int loser;
	int winner;
	int listid;
	struct loser_tree_node *parent, *left, *right;
};

class LoserTree
{
	public:
		int level = 0;
		int filenum;
		loser_tree_node *root;
		vector<loser_tree_node> loser_tree;
		vector<vector<trace>> *filetop;

		LoserTree(int fn, vector<vector<trace>> *ft);
		void CreateLoserTree();
		void NodeCompare(int listid);
		void PrintTreeNodes();
		void PrintTreeNode(int listid);
		int GetWinner();
		void UpdateTree();
		void UpdateTreeNode(int listid);
};

#endif