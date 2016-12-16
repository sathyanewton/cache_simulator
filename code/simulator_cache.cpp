#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <cmath>
#include <vector>

using namespace std;

/* skeleton frame work */

/*class block {
      unsigned long tag
      unsigned long timestamp
      char valid
      update_ts(unsigned long newts){update timestamp}
      update_tag(unsigned long newtag){update tag}
      int compare_tag(unsigned long tag){compare tag with my tag, return block id or "miss")
}

class set {
      int num_blks
      block *blk
      char replacement_policy
      int access(tag){compare tag with blocks in this set, return block id or "miss"}
      int find_victim(policy){find a victim block to replace with "policy"}
}

class cache {
      unsigned long total_access_counter, hit_count
      int cache_size
      int block_size
      int set_associativity
      int num_sets
      set *sets
      char replacement_policy
      int access(addr){locate and access a set, return "hit" or "miss"}
      cache(cache_size, block_size, set_assoc, replacement_policy){constructor of the cache class}
}
*/


/*Global variables and data structures*/

struct block
{
		unsigned int stored_tag;
		int valid;
		int dirty;
		int block_is_full;
		int lru_count;
};

struct set
{
	block *blk;
	int num_blocks;
	int set_is_full;
	vector<int>lru_vector;
	
};

struct cache
{
	set *set_pointer;
	int total_num_sets;
};

struct cache cache;
int hits=0;
int read_hits=0;
int write_hits=0;
int miss=0;
int read_miss=0;
int write_miss=0;

unsigned int find_number_of_bits(int n)
{
	unsigned int a=0,temp=1;
	while(1)
	{
		if(n<=temp)
		{
		break;
		}
		else
		{
        temp=temp<<1;
        a++;
		}
	}

	return a;
}

unsigned int pharse_address_tag(unsigned int address,int number_of_bits_of_offset,int number_of_bits_of_index)
{
	unsigned int tag;
	tag=address>>(number_of_bits_of_offset+number_of_bits_of_index);
	cout<<"address -> "<<address<<" "<<"tag -> "<<tag<<" ";
	return tag;
}

unsigned int pharse_address_index(unsigned int address,int number_of_bits_of_offset,int number_of_bits_of_index)
{
	unsigned int index,temp;
	temp=address>>number_of_bits_of_offset;

	index=((temp)&((1<<number_of_bits_of_index)-1));

	cout<<"index -> "<<index<<" ";

	return index;
}

unsigned int pharse_address_offset(unsigned int address,int number_of_bits_of_offset,int number_of_bits_of_index)
{
	unsigned int offset;

	offset = ((address)&((1<<number_of_bits_of_offset)-1));

	cout<<"offset -> "<<offset<<"\n";

	return offset;
}

void initialize_struct_variables(int s,int e)
{
	int i=0,j=0;
	for(i=0;i<s;i++)
	{
		cache.set_pointer[i].num_blocks=0;
		cache.set_pointer[i].set_is_full=0;
		for(j=0;j<e;j++)
		{
		cache.set_pointer[i].blk[j].stored_tag=0;
		cache.set_pointer[i].blk[j].valid=0;
		cache.set_pointer[i].blk[j].block_is_full=0;
		}
	}
}

void lru_push(int index,int i)
{
	cout<<" inside lru_push function "<<"\n";
	cache.set_pointer[index].lru_vector.push_back(i);

	/*
	int k=0;
	cache.set_pointer[index].lru_vector.push_back(i);
	for(k=0;(k<cache.set_pointer[index].lru_vector.size()-1);k++)
	{
	if(cache.set_pointer[index].lru_vector[k] == i)
	{
		cache.set_pointer[index].lru_vector.erase(cache.set_pointer[index].lru_vector.begin()+k);

	}
	}*/
}
void access_cache(unsigned int tag,unsigned int index,unsigned int offset,int e,char c,int number_of_bits_of_index)
{	
	//cout<<"e value : "<<e<<"\n";
	int i=0,counter=0,tagfound=0,k=0,p=0,l=0,m=0,maxval=0,location=0;
	
	if(index<250)											//generalize this at the end!!!!!!!!!!!!!!!!!
	{
	if(cache.set_pointer[index].set_is_full==1)
	{
		maxval=cache.set_pointer[index].blk[0].lru_count;
		cout<<"set is full; have to perform LRU\n";
		miss++;
			if(c == 'L')
			{
				read_miss++;
			}
			else if(c == 'S')
			{
				write_miss++;
			}
			else if(c == 'M')
			{
				write_miss++;
			}
		for(l=1;l<e;l++)
		{
			if(cache.set_pointer[index].blk[l].lru_count>maxval)
			{
				location=l;
			}
		}
		cache.set_pointer[index].blk[location].stored_tag=tag;

	}	
	else
	{
		for(i=0;i<e;i++)
		{
			if(cache.set_pointer[index].blk[i].block_is_full==0)
			{
				cout<<"inside if of access"<<"\n";
				cache.set_pointer[index].blk[i].stored_tag=tag;
				cache.set_pointer[index].blk[i].block_is_full=1;
				break;
			}
			else
			{
				counter++;
				if(counter==(e-1))
				{
					cache.set_pointer[index].set_is_full=1;
				}
				else if(tag==cache.set_pointer[index].blk[i].stored_tag)
				{
					cache.set_pointer[index].blk[i].lru_count++;
					hits++;
					tagfound=1;
					if(c == 'L')
					{
						read_hits++;
					}
					else if(c == 'S')
					{
						write_hits++;
					}
					else if(c == 'M')
					{
						write_hits++;
					}
					//lru_push(index,i);
					break;
				}
			} 
		}
		if(tagfound==0)
		{
			miss++;
			if(c == 'L')
			{
				read_miss++;
			}
			else if(c == 'S')
			{
				write_miss++;
			}
			else if(c == 'M')
			{
				write_miss++;
			}
		
		}
	}
}
}
int main()
{
	int block_size,assoc,cache_size,total_num_blocks;
	int s,e,b;
	unsigned int number_of_bits_of_offset,number_of_bits_of_index;
	int function_output,funtion_input;		//debugging stub variables
	unsigned int tag,index,offset;
	unsigned int address;
	int i,j,k;								//iterators 
	int stub_counter=0;




	cout<<"choose from pre-programmed architectures :"<<"\n";
	cout<<"1.Nahelam"<<"\n";					// L1 cache size is 64KB


	cout<<"enter the cache size : ";			//64KB for nahlem
	cin>>cache_size;
	//cout<<"\n";

	cout<<"Enter Blocksize : ";
	cin>>block_size;    						//also known as cache line size (typically equal to 64 bytes in a 64 byte processor(i-7))
	//cout<<"\n";

	cout<<"Enter associvity : ";
	cin>>assoc;


	b=block_size;	

	e=assoc; //clines in a set

	s = ((cache_size)/(b*e));			//total number of sets

	total_num_blocks = (cache_size/b);	//total number of blocks in the cache
	/*		s = 250 ; e = 4 ;
	struct block
{
		unsigned int stored_tag;
		int valid;
		int dirty;
		int is_full;
};

struct set
{
	block *blk;
	int num_blocks;
	
};

struct cache
{
	set *set_pointer;
	int total_num_sets;
};
	*/

	/************ MEMORY ALLOCATION **************/
	cout<<"s value : "<<s<<"\n";

	cache.set_pointer=(set*)malloc(sizeof(set)*s);

	for(i=0;i<s;i++)
	{
	cache.set_pointer[i].blk=(block*)malloc(sizeof(block)*e);
	}
	/**********************************************/

	/* STUB 1 */
	//cout<<"funtion_input : ";
	//cin>>funtion_input;
	//function_output = find_number_of_bits(funtion_input);
	//cout<<function_output<<"\n"

	initialize_struct_variables(s,e);
	number_of_bits_of_offset = find_number_of_bits(b);
	number_of_bits_of_index = find_number_of_bits(s);
	cout<<"number_of_bits_of_offset : "<<number_of_bits_of_offset<<"\n";
	cout<<"number_of_bits_of_index : "<<number_of_bits_of_index<<"\n";

	FILE *file;
	char Name[] = "trace.txt";
	char opcode;
	
	file = fopen(Name,"r");
	if( file == NULL)
	{
		cout<<"File not Found";
		exit(-1);
	}
	char c = 'z';						// Dummy Initialization
	char l;
	while (1)
	{
		while ((c = getc(file)) != 'S' && (c != EOF) && (c != 'M')&& (c != 'L') );
		
		
			if( c == EOF )
			{
			break;
			}
			else
			{
				opcode=c;
				fscanf(file,"%x",&address);
				//cout<<"Model for address translation, a sort of a stub\n";
				cout<<"address :"<<address<<"\n";
				tag = pharse_address_tag(address,number_of_bits_of_offset,number_of_bits_of_index);
				index=pharse_address_index(address,number_of_bits_of_offset,number_of_bits_of_index);
				offset=pharse_address_offset(address,number_of_bits_of_offset,number_of_bits_of_index);
				access_cache(tag,index,offset,e,c,number_of_bits_of_index);
				stub_counter++;
				if(stub_counter==1000)
				{
				//break;
				}
			}
			
		
	}

cout<<"total number of Hits : "<<hits<<"\n";
cout<<"total number of Read Hits : "<<read_hits<<"\n";
cout<<"total number of Write Hits : "<<write_hits<<"\n";
cout<<"total number of Misses : "<<miss<<"\n";
cout<<"total number of Read Misses : "<<read_miss<<"\n";
cout<<"total number of Write Misses : "<<write_miss<<"\n";

	return 0;
}