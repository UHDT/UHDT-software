void states(int *a,int *b,int *c,int *d,int *e,int *f,int *g,int *h,int *i)
{
int save = *a;
	*a = *b;
	*b = *c;
	*c = save;

int save1 = *d;
	*d = *e;
	*e = *f;
	*f = *g;
	*g = *h;
	*h = *i;
	*i = save1;

}
