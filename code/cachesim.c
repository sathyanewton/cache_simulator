// LRU_cache.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <cmath>

/********************************* global variable declaration *****************************/

using namespace std;
int cache_size,assoc,set_number,block_size;
int b,s,e;
int B,S,E;
int f=0;
int g=0;
//int LRU=0;
int wpolicy;
unsigned int memory[1000];			// Memory Location of Operation
	int val[1000];					// Value to be operated
	char op[1000];					// Operation ( S, M, L)
	 int n = 0;						// Index 
	char offset[1000];					// First 5 bits ( From LSB )
	int index[1000];					// Next 8 bits
	int tag_data[1000];
	int tag[1000];						// Rest 19 bits
	char junk;
	char buffer [33];
	int victim_block_size;
	int k;
unsigned int idx_data[1000];
typedef struct block;
typedef struct cline;
typedef struct set;
typedef struct cache;
typedef struct victim;
typedef struct l2block;
typedef struct l2cache;
/***************************************** file pharse ******************************************/
void init()
{
FILE *file;
	char Name[] = "trace.txt";
	
	file = fopen(Name,"r");
	if( file == NULL)
	{
		cout<<"File not Found";
		exit(-1);
	}
	char c = 'z';						// Dummy Initialization
	
	while (1)
	{
		while ((c = getc(file)) != 'S' && (c != EOF) && (c != 'M')&& (c != 'L') );
		
		if( c == EOF )
			break;

		else
		{
			fscanf(file,"%c",&junk);
			//cout<<c<<' ';
		
			if ( c == 'L' )
			{
				op[n] = c;
				fscanf(file,"%c",&c);
				fscanf(file,"%x",&memory[n]);
				fscanf(file,"%c",&c);
				fscanf(file,"%d",&val[n]);
				//printf("%x : %d\n",memory[n],val[n]);
			// Call load function here
			}
			else if ( c == 'S' )
			{
				op[n] = c;
				fscanf(file,"%c",&c);
				fscanf(file,"%x",&memory[n]);
				fscanf(file,"%c",&c);
				fscanf(file,"%d",&val[n]);
				//printf("%x : %d\n",memory[n],val[n]);
			// Call store function here
			}
			else if ( c == 'M' )
			{
				op[n] = c;
				fscanf(file,"%c",&c);
				fscanf(file,"%x",&memory[n]);
				fscanf(file,"%c",&c);
				fscanf(file,"%d",&val[n]);
				//printf("%x : %d\n",memory[n],val[n]);
			// Call Modify function here
			}		
		}
	


//		itoa (memory[n],buffer,2);
		//printf ("Adress in binary: %s\n",buffer);
		//offset[n] = memory[n] & 0x0000001f;
		//index[n] = (memory[n] & 0x00001fe0) >> 5;
		//tag[n] = (memory[n] & 0xffffe000) >> 13;
	//printf("%x\n",memory[n]);
	
		n++;
		
	}
	//cout<<n<<"\n";
}

/*********************** DATA PHARSING *********************************/

void pharse_address()
{ 
	int y;
	int z;
	for(int i=0;i<n;i++)
	{
		z=B+S;
		tag_data[i]=memory[i]>>z;						//right shifted by (B+S) number of bits to remove the tag
		y = (e*(tag_data[i]&((1<<S) - 1)));				//y is our index. 
		if(y>s)
		{
			y=s;
		}
		idx_data[i]=y;
		
		//printf("%d\n",idx_data[i]);	
		//y=i;
	}
}

/****************************************** STRUCT DEFINITION ********************************/
struct block
{
		int valid;
		int tag;
		int lrucount;
};
struct cline
{
	block blk1;
	
	
};
struct set
{
	cline *cl1;
	int set_index;
	
};
struct cache
{
	set *s1;
	int hit;
	int miss;
	int read_hit;
	int read_miss;
	int write_hit;
	int write_miss;
	int access;
	int number_of_reads;
	int number_of_writes;
	
};

struct vblock
{
	int victim_dirty;
	int victim_tag;
	
};
struct victim 
{
	vblock *vb;
	int victim_hit;
	int victim_miss;
	int victim_read_hit;
	int victim_read_miss;
	int victim_write_hit;
	int victim_write_miss;
};
struct l2block
{
	int l2_tag;
	int l2_valid;

};
struct l2cache
{
	l2block *l2_block;
	int l2_victim_hit;
	int l2_victim_miss;
};

struct cache real_cache;
struct victim real_victim_cache;
struct l2cache real_l2_cache;
/************************************* INITIALIZING STRUCT VARIABLES **************************************/
void initialize_struct_variables()
{
	real_cache.hit=0;
	real_cache.miss=0;
	real_cache.access=0;
	real_cache.number_of_reads=0;
	real_cache.number_of_writes=0;
	real_cache.write_hit=0;
	real_cache.read_hit=0;
	
	
real_victim_cache.victim_hit=0;
	real_victim_cache.victim_miss=0;
	real_victim_cache.victim_read_hit=0;
	real_victim_cache.victim_read_miss=0;

	for(int i=0;i<s;i++)
	{
		for(int j=0;j<assoc;j++)
		{
			real_cache.s1[i].cl1[j].blk1.tag=0;
			real_cache.s1[i].cl1[j].blk1.valid=0;
			real_cache.s1[i].cl1[j].blk1.lrucount=0;
		}
	}
}


/**************************************** READ CACHE FUNCTION ***************************************/

void read_cache()
{
	
	for(int i=0;i<n;i++)
	{
		for(int y=0;y<=s;y++)
		{
		if(idx_data[i]==real_cache.s1[y].set_index)
		{
			for(int j=0;j<assoc;j++)
			{
			if(real_cache.s1[y].cl1[j].blk1.valid==0)
			{
				real_cache.miss++;
				real_cache.s1[y].cl1[j].blk1.valid=1;
			
				for(int k=0;k<assoc;k++)
				{
					if(k!=j)
					{
				real_cache.s1[y].cl1[k].blk1.lrucount++;
					}
				}
				real_cache.s1[y].cl1[j].blk1.tag=tag_data[i];
				if(op[i] == 'L')
				{
					real_cache.read_miss++;
				}
				else if(op[i]=='S')
				{
					real_cache.write_miss++;
				}
				else if(op[i]=='M')
				{
					real_cache.write_miss++;
				}
				
				break;
			}
			else if (real_cache.s1[y].cl1[j].blk1.valid==1)
			{
				if(real_cache.s1[y].cl1[j].blk1.tag==tag_data[i])
				{
					real_cache.hit++;
				if(op[i] == 'L')
				{
					real_cache.read_hit++;
				}
				else if(op[i]=='S')
				{
					real_cache.write_hit++;
				}
				else if(op[i]=='M')
				{
					real_cache.write_hit++;
				}	
					
				}
				else if(real_cache.s1[y].cl1[j].blk1.tag!=tag_data[i])
				{
					real_cache.miss++;
					
						int z=j;
						int temp;
					/***************************** LRU ****************************************/
						for(int q=0;q<s;q++)
						{
							temp=real_cache.s1[y].cl1[q].blk1.lrucount;
							for(int v=0;v<=s;v++)
							{
							if(real_cache.s1[y].cl1[v].blk1.lrucount>temp)
							{
								z=v;
							}
							}
						}
						/*************************************************************/
					if(j<(assoc-1))
					{
						real_cache.s1[y].cl1[z].blk1.tag=tag_data[i];
						for(int k=0;k<assoc;k++)
						{
							if(k!=j)
							{
								real_cache.s1[y].cl1[k].blk1.lrucount++;
							}
						}
				
					}
					else if(j>=(assoc-1)) 
					{
						real_cache.miss++;
					if(op[i] == 'L')
					{
						real_cache.read_miss++;
					}
					else if(op[i]=='S')
					{
					real_cache.write_miss++;
					}
					else if(op[i]=='M')
					{
					real_cache.write_miss++;
					}
						real_victim_cache.vb[f].victim_tag=real_cache.s1[y].cl1[j].blk1.tag;
						real_victim_cache.vb[f].victim_dirty=1;
						f++;
					}
				}
				}
			}
			}
		}

	}
}

/*********************************** DIRECT MAP ****************************************/

void read_direct_cache()
{
	for(int i=0;i<n;i++)
	{
		for(int n1=0;n1<s;n1++)
		{
		if(index[i]==real_cache.s1[n1].set_index)
		{
			for(int j=0;j<assoc;j++)
			{
				real_cache.s1[n1].cl1[j].blk1.valid=1;
			
			if(tag[i]==NULL)
			{
				real_cache.s1[n1].cl1[j].blk1.tag=tag_data[i];
				real_cache.miss++;
			}
			else if(real_cache.s1[n1].cl1[j].blk1.tag=tag_data[i])
			{
				real_cache.hit++;
			}
			else if(real_cache.s1[n1].cl1[j].blk1.tag=tag_data[i])
			{
				real_cache.miss++;
				real_cache.s1[n1].cl1[j].blk1.tag=tag_data[i];
			}
			}
		}
		}
	}
}
/********************************************** VICTIM CACHE READ **************************/
void read_victim_cache()
{
for(int j=0;j<n;j++)
	{
	for(int i=0;i<=8;i++)
	{
		if(i<(victim_block_size-1))
		{
		if(real_victim_cache.vb[i].victim_dirty==1)
		{
			
		
			if(real_victim_cache.vb[i].victim_tag==tag_data[j])
			{
				real_victim_cache.victim_hit++;
				if(op[j]='L')
				{
					real_victim_cache.victim_read_hit++;
				}
				else if(op[j]='S')
				{
					real_victim_cache.victim_write_hit++;
				}
				else if(op[j]='M')
				{
					real_victim_cache.victim_write_hit++;
				}
				break;
			}
			else if(real_victim_cache.vb[i].victim_tag!=tag_data[j])
			{
				real_victim_cache.victim_miss++;
				if(op[j]='L')
				{
					real_victim_cache.victim_read_miss++;
				}
				else if(op[j]='S')
				{
					real_victim_cache.victim_write_miss++;
				}
				else if(op[j]='M')
				{
					real_victim_cache.victim_write_miss++;
				}
				break;
			}
			
			
		}
			else if(real_victim_cache.vb[i].victim_dirty==0)
			{
				real_victim_cache.victim_miss++;
				if(op[j]='L')
				{
					real_victim_cache.victim_read_miss++;
				}
				else if(op[j]='S')
				{
					real_victim_cache.victim_write_miss++;
				}
				else if(op[j]='M')
				{
					real_victim_cache.victim_write_miss++;
				}
				break;
			}
		}
		else 
		{
			real_l2_cache.l2_block[g].l2_tag=real_victim_cache.vb[i].victim_tag;
			g++;
		}
		}
		}
	}








/**************************************************************************/
int main()
{
/************************************** USER INPUT AND CALCULATION *************************/
	
	init();
	cout<<"enter the cache size in \n";
	cin>>cache_size;
	cout<<"enter associvity value\n";
	cout<<"its either 1 2 or 4\n";
	cin>>assoc;
	cout<<"enter the block size\n";
	cin>>block_size;
	b=block_size;	
	e=assoc; //clines in a set
	set_number = ((cache_size)/(b*e));
	s=set_number; //sets in total
	cout<<"enter victim block cache size\n";
	cin>>victim_block_size;
	cout<<"specifiy write policy";
	cout<<"1.write back"<<"\n";
	cout<<"2.write allocate"<<"\n";
	cin>>wpolicy;
	

	int temp=1,b1,s1;
	b1=b;
	while(1)
	{
		if(b1==temp)
		{
		break;
		}
		else
		{
        temp=temp<<1;
        B++;
		}
	}
	//cout<<B<<"\n";	//number of bits in block offset
	temp=1;
	 s1=s;
	while(1)
	{
		if(s1==temp)
		{
		break;
		}
		else
		{
        temp=temp<<1;
        S++;
		}
	}
	//cout<<S<<"\n";	//number of bits in an index
	temp=0;
	/*********************************** CALLING ADDRESS PHARSING  ************************/

	pharse_address();

	/*********************************** MEMORY ALLOCATION *********************************/


	//real_cache=(cache*)malloc (sizeof (struct cache));
	real_cache.s1=(set*)malloc(sizeof(set)*s);
	for(int i=0;i<s;i++)
	{
	real_cache.s1[i].cl1=(cline*)malloc(sizeof(cline)*e);
	}
	for(int i=0;i<s;i++)
	{
		for(int j=0;j<assoc;j++)
		{
		//real_cache.s1[i].cl1[j].blk1=(block*)malloc(sizeof(block));
		}
	}
	
	real_victim_cache.vb=(vblock*)malloc(sizeof(victim_block_size)*8);
	
	real_l2_cache.l2_block=(l2block*)malloc(sizeof(cache)*8);

	/************************************** CALLING INITIALIZE_STRUCT_VARIABLES ****************/
	initialize_struct_variables();
	
	
	/*******************************************************************************************/

	for(int i=0;i<s;i++)
	{
		real_cache.s1[i].set_index=i;
	//	cout<<real_cache.s1[i].set_index<<"\n";
	}
	
	/*************************************************************************************************/
	
	for(int i=0;i<=n;i++)
	{
		
		//cout<<idx_data[i]<<"\n";
	}

	read_cache();
	read_victim_cache();
	cout<<"L1 cache hits"<<real_cache.hit<<"\n";
	cout<<"L1 cache misses"<<real_cache.miss<<"\n";
	cout<<"L1 cache read hits"<<real_cache.read_hit<<"\n";
	cout<<"L1 cache read miss"<<real_cache.read_miss<<"\n";
	cout<<"L1 cache reads"<<(real_cache.read_hit+real_cache.read_miss)<<"\n";
	cout<<"L1 cache write hits"<<real_cache.write_hit<<"\n";
	cout<<"L1 cache write miss"<<real_cache.write_miss<<"\n";
	cout<<"L1 cache writes"<<(real_cache.write_hit+real_cache.write_miss)<<"\n";
	cout<<"victim hits"<<real_victim_cache.victim_hit<<"\n";
	cout<<"victim misses"<<real_victim_cache.victim_miss<<"\n";
	cout<<"Victim cache read hits"<<real_victim_cache.victim_read_hit<<"\n";
	cout<<"Victim cache write hits"<<real_victim_cache.victim_write_hit<<"\n";
	cout<<"Victim cache read miss"<<real_victim_cache.victim_read_miss<<"\n";
	cout<<"Victim cache write miss"<<real_victim_cache.victim_write_miss<<"\n";
	for(int k=0;k<n;k++)
	{
		//cout<<tag_data[k]<<'\n';
	}
	for(int i=0; i<s ;i++)
	{
		for(int j=0;j<e;j++)
		{	
			//cout<<"TAG and VALID"<<"/n";
			 //cout<<real_cache.s1[i].cl1[j].blk1.tag<<' '<<real_cache.s1[i].cl1[j].blk1.valid<<'\n';
			
		}
	}
	for(int i=0;i<victim_block_size;i++)
	{
		//cout<<real_victim_cache.vb[i].victim_tag<<"\n";
	}
	for(int i=0;i<=8;i++)
	{
		//cout<<real_l2_cache.l2_block[i].l2_tag<<"\n";
	}
	return 0;
}

