/* POJ 3481 - Double Queue, by Abreto<m@abreto.net>. */
#include <cstdio>

using namespace std;

#define MAKE_RE { ((void (*)(void))NULL)(); }

#define MAXMEM	(1000100)
int __my_mem[MAXMEM];
inline void initmem(void)	/* Don't forget!! */
{
	for(int i = 0;i < MAXMEM;i++)
		__my_mem[i] = i+1;
}
inline int new_address(void)
{
	int ret = __my_mem[0];
	if(ret == MAXMEM) MAKE_RE;
	__my_mem[0] = __my_mem[ret];
	return ret;
}
inline void free_address(int p)
{
	__my_mem[p] = __my_mem[0];
	__my_mem[0] = p;
}

#define LC(x)	((x)->ch[0])
#define RC(x)	((x)->ch[1])
#define SZ(x)	((x)?((x)->sz):(0))
struct node
{
	int key;	/* priority */
	int val;	/* client */
	node *fa;	/* father */
	node *ch[2];	/* 0 for left child, 1 for right child. */
	int sz;		/* the size of tree rooted at this. */
}nodes[MAXMEM];
inline node *new_node(int k, int v)
{
	node *ret = nodes + new_address();
	ret->key = k;
	ret->val = v;
	ret->fa = ret->ch[0] = ret->ch[1] = NULL;
	ret->sz = 1;
	return ret;
}
inline void free_node(node *p)
{
	free_address( p - nodes );
}
inline void maintain(node *p)
{
	if( p )
	{
		p->sz = SZ(LC(p)) + SZ(RC(p)) + 1;
	}
}
inline void mkchild(node *f, int d, node *p)	/* make p the d-th child of f */
{
	if ( f ) { f->ch[d] = p; maintain(f); }
	if ( p ) { p->fa = f; }
}
inline int which(node *p)	/* return 1 if p is a right child or 0 if left. */
{
	return ( (p) && (p->fa) ) ? ( RC(p->fa) == p ) : -1;
}
inline node *rotate(node *p)	/* rotate p to its father if exists */
{
	if( NULL == p ) return NULL;
	node *f = p->fa;
	if ( NULL == f ) return p;
	int d = which(p);
	node *ff = f->fa; int df = which(f);
	mkchild(f, d, p->ch[d^1]);
	mkchild(p, d^1, f);
	p->fa = ff;
	if ( ff ) ff->ch[df] = p;
	return p;
}
inline node *splay(node *p, node *root)	/* rotate p to root. */
{
	node *fr = root->fa;
	while( p->fa != fr )
	{
		node *f = p->fa;
		if( f->fa != fr )
		{
			rotate( (which(p) == which(f)) ? f : p );
		}
		rotate(p);
	}
	return p;
}
inline node *get_k_th(node *root, int k)
{
	node *p = root;
	int rank;
	while( k != (rank = (SZ(LC(p)) + 1)) )
	{
		if( k < rank ) p = LC(p);
		else { k -= rank; p = RC(p); }
	}
	return splay(p, root);
}
inline node *myins(node *root, int K, int P)	/* client K with priority P */
{
	node *p = root;
	node *f = p ? p->fa : NULL;
	while( p )
	{
		f = p;
		if( P < p->key ) p = LC(p);
		else if ( P > p->key ) p = RC(p);
		else MAKE_RE;
	}
	p = new_node(P, K);
	mkchild(f, f ? (P > f->key) : -1, p);
	return root ? splay(p, root) : p;
}
inline node *delrt(node *root)	/* delete root node */
{
	node *lc = LC(root), *rc = RC(root);
	node *newroot;
	if ( rc )
	{
		rc = get_k_th(rc, 1);
		/* now rc has no left child */
		mkchild(rc, 0, lc);
		newroot = rc;
	} else {
		newroot = lc;
	}
	if ( newroot ) newroot->fa = NULL;
	return newroot;
}

int main(void)
{
	node *stree = NULL;
	int command = 0;
	initmem();
	while( scanf("%d", &command) && command )
	{
		if(1 == command)
		{
			int K, P;
			scanf("%d%d", &K, &P);
			stree = myins(stree, K, P);
		}
		else
		{
			if( NULL == stree ) { puts("0"); continue; }
			if (2 == command)
			{
				stree = get_k_th(stree, stree->sz);
		   	}
			else if (3 == command)
			{
			   stree = get_k_th(stree, 1);
			}
			else MAKE_RE;
			printf("%d\n", stree->val);
			stree = delrt(stree);
		}
	}
	return 0;
}
