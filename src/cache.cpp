#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <list>
#include <unordered_map>
using namespace std;

class LRUCache
{
    list<int> dq;
    unordered_map<int, list<int>::iterator> ma;
    int csize; 

public:
    LRUCache(int x = 4);
    void refer(int);
    int result();
    int print;
};

LRUCache::LRUCache(int n)
{
    csize = n;
}

int LRUCache::result(void)
{
    return print;
}

void LRUCache::refer(int x)
{
    if (ma.find(x) == ma.end())
    {
        if (dq.size() == csize)
        {
            int last = dq.back();
            print = last;
            dq.pop_back();
            ma.erase(last);
        }
        else
            print = -1;
    }
    else
    {
        dq.erase(ma[x]);
        print = -1;
    }
    dq.push_front(x);
    ma[x] = dq.begin();
} 

int main(int argc, char *argv[])
{
    int i, j, k, cache, block, assoc, replace, block_num, set, min;
    long int address;
    int result = -1;
    int tag_addr = 0, index_addr = 0;
    int index = 0, offset = 0;
    string txt, cache_size, block_size, assoc_a, replace_r, address_a;
    fstream read;
    fstream write;
    txt = argv[1];
    read.open(txt, ios::in);
    txt = argv[2];
    write.open(txt, ios::out);
    getline(read, cache_size);
    getline(read, block_size);
    getline(read, assoc_a);
    getline(read, replace_r);
    cache = stoi(cache_size);
    block = stoi(block_size);
    assoc = stoi(assoc_a);
    replace = stoi(replace_r);
    block_num = cache * 1024 / block;
    if(assoc == 0){
        index = log2(block_num);
        offset = log2(block);
        int array[block_num][1];
        for(i = 0; i < block_num; ++i)
        {
                array[i][0] = -1;
        }
        if(replace == 0 || replace == 1 || replace == 2)
        {
            while(getline(read, address_a))
            {
                address = strtol(address_a.c_str(), nullptr, 16);
                index_addr = (address >> offset) % block_num;
                tag_addr = address >> (index + offset);
                if(array[index_addr][0] == -1)
                {
                    array[index_addr][0] = tag_addr;
                    write << result << endl;
                }
                else
                {
                    if(array[index_addr][0]==tag_addr)
                        write << result << endl;
                    else
                    {
                        write << array[index_addr][0] << endl;
                        array[index_addr][0] = tag_addr;
                    }
                }
            }
        }
    }    
    else if(assoc == 1)
    {
        set = block_num/4;
        offset = log2(block);
        index = log2(set);
        if(replace == 0)
        {
            int array[set][5];
            int same = 0, out = 1;
            for(i = 0; i < set; ++i)
            {
                for(j = 0; j < 4; ++j)
                {
                    array[i][j] = -1;
                }
                array[i][4] = 0;
            }
            while(getline(read, address_a))
            {
                address = strtol(address_a.c_str(), nullptr, 16);
                index_addr = (address >> offset) % set;
                tag_addr = address >> (index + offset);
                for(i = 0; i < 4; ++i)
                {
                    if(array[index_addr][i] == tag_addr)
                    {
                         write << result << endl;
                         same = 1;
                         break;
                    }
                    same = 0;
                }
                if(same == 0)
                {
                    for(i = 0; i < 4; ++i)
                    {
                        if(array[index_addr][i] == -1)
                        {
                            array[index_addr][i] = tag_addr;
                            write << result << endl;
                            out = 0;
                            break;
                        }
                        out = 1;
                    }
                    if(out == 1)
                    {
                        write << array[index_addr][array[index_addr][4]] << endl;
                        array[index_addr][array[index_addr][4]] = tag_addr;
                        if(array[index_addr][4] == 3)
                            array[index_addr][4] = 0;
                        else
                            array[index_addr][4] += 1;
                    }
                }
            }
        }
        if(replace == 1 || replace == 2)
        {
            LRUCache lru[set];
            while(getline(read, address_a))
            {
                address = strtol(address_a.c_str(), nullptr, 16);
                index_addr = (address >> offset) % set;
                tag_addr = address >> (index + offset);
                lru[index_addr].refer(tag_addr);
                write << lru[index_addr].result() << endl;
             }

        }
        /*if(replace == 2)
        {
            int array[set][4];
            int set_number;
            int a, flag = 0, sign;
            srand(time(NULL));
            for(i = 0; i < set;++i)
            {
                for(j=0;j<4;j++)
                {
                    array[i][j]=-1;
                }
            }
            while(getline(read, address_a))
            {
                address = strtol(address_a.c_str(), nullptr, 16);
                index_addr = (address >> offset) % set;
                tag_addr = address >> (offset + index);
                a = (rand()%4);
                sign = 0;
                for(i=0; i <4 ;i++)
                {
                    if(array[index_addr][i]==tag_addr)
                    {
                        write << result << endl;
                        sign = 1;
                        break;
                    }
                    else
                        sign = 0;
                }
                if(sign == 0)
                {
                    flag = 0;
                    for(i = 0; i < 4; ++i)
                    {
                        if(array[index_addr][i] == -1)
                        {
                            array[index_addr][i] = tag_addr;
                            write << result << endl;
                            flag = 1;
                            break;
                        }
                        else
                            flag = 0;
                    }
                }
                if(flag == 0)
                {
                    write << array[index_addr][a] << endl;
                    array[index_addr][a] = tag_addr;
                }
            }
        }*/
    }
    else if(assoc == 2)
    {
        offset = log2(block);
        if(replace == 0)
        {
            int array[1][block_num];
            int sign = 0, mark = 0, substitute = 0;
            for(i = 0; i < block_num; ++i)
            {
                array[0][i] = -1;   
            }
            while(getline(read, address_a))
            {
                address = strtol(address_a.c_str(), nullptr, 16);
                tag_addr = address >> offset;
                for(i = 0; i < block_num; ++i)
                {
                    if(array[0][i] == tag_addr)
                    {
                        write << result << endl;
                        sign = 1;
                        break;
                    }
                    sign = 0;
                }
                if(sign == 0)
                {
                    for(i = 0; i < block_num; ++i)
                    {
                        if(array[0][i] == -1)
                        {
                            array[0][i] = tag_addr;
                            write << result << endl;
                            mark = 1;
                            break;
                        }
                        mark = 0;
                    }
                    if(mark == 0)
                    {
                        write << array[0][substitute] << endl;
                        array[0][substitute] = tag_addr;
                        if(substitute == block_num - 1)
                            substitute = 0;
                        else
                            substitute +=1;
                    }
                }
            }
        }
        if(replace == 1 || replace == 2)
        {
            LRUCache lru(block_num);
            while(getline(read, address_a))
            {
                address = strtol(address_a.c_str(), nullptr, 16);
                tag_addr = address >> offset;
                lru.refer(tag_addr);
                write << lru.result() << endl;
            }
        }
    }
    read.close();
    write.close();
    return 0;
}
