#ifndef NODECLASS
#define NODECLASS

class node
{
    public:
	node* leftNode = 0;
    node* rightNode = 0;
    node* rootNode = 0;
    int label = 0; 
    double splitval = 0, attr = 0;

};

#endif 