#include<bits/stdc++.h>

using namespace std;

int global_depth, bucket_capacity;

int remove_msb(int i){
    int n = i, cnt =0;
    while(n>0){
        cnt++;
        n = n>>1;
    }
    return i - (1<<(cnt-1));
}

int add_one_to_msb(int i, int j){
    // cout<<i<<" "<<j<<endl;
    // cout<<i + 1<<j<<endl;
    return i + (1<<j);
}

bool isPresent(vector<int> &v, int value){
    for(int i=0;i<v.size();i++)
    {
        if(v[i]==value){
            return true;
        }
    }
    return false;
}

bool isDistributed(vector<int> &v, int local_depth){
    int tmp = 1<<(local_depth);
    tmp--;
    map<int,int> m;
    for(int i=0;i<v.size();i++){
        m[tmp&(v[i])]++;
    }
    if(m.size()>1){
        return true;
    }else{
        return false;
    }
}

int main(){
    cin>>global_depth>>bucket_capacity;
    // cout<<add_one_to_msb(6,3)<<endl;;
    int initial_size = 1<<global_depth;
    vector<pair<int,vector<int>>> buckets(initial_size,make_pair(global_depth,vector<int> ()));
    vector<int> order;

    //pushing the iniital bucket in the default order
    for(int i =0;i<(1<<global_depth);i++){
        order.push_back(i);
    }

    // cout<<buckets.size()<<endl;
    // cout<<buckets[0].second.size()<<endl;
    int input,value;
    while(1){
        cin>>input;
        if(input==2){
            cin>>value;
            // cout<<value<<"\n";
            //insert the value in the extendible hash map
            // int 
            int bucket_idx = 1<<(global_depth);
            bucket_idx--;
            bucket_idx &= value;
            // cout<<bucket_idx<<" ";
            // cout<<buckets[bucket_idx].first<<endl;
            while((buckets[bucket_idx].first)==0){
                bucket_idx = remove_msb(bucket_idx);
            }
            if(buckets[bucket_idx].second.size()!=bucket_capacity){
                // cout<<"hello in "<<bucket_idx<<endl;
                // cout<<"now the size is: "<<buckets[bucket_idx].second.size()<<endl;
                buckets[bucket_idx].second.push_back(value);
                if(!isPresent(order,bucket_idx)){
                    order.push_back(bucket_idx);
                }
                // cout<<"global depth: "<<global_depth<<endl;
                // for(int i=0;i<(1<<global_depth);i++){
                //     cout<<i<<" "<<"local depth: "<<buckets[i].first<<endl;
                //     for(int j = 0;j<buckets[i].second.size();j++){
                //         cout<<buckets[i].second[j]<<" ";
                //     }
                //     cout<<endl;
                // }
            }else{
                int local_depth = buckets[bucket_idx].first;
                cout<<local_depth<<endl;
                while(global_depth<=20){
                    if(local_depth<global_depth){
                        // int buffer_for_extra_unused_bucket = local_depth;
                        local_depth++;
                        vector<int> tmp_bucket;
                        tmp_bucket.push_back(value);
                        for(int i=0;i<bucket_capacity;i++){
                            tmp_bucket.push_back(buckets[bucket_idx].second[i]);
                        }
                        if(isDistributed(tmp_bucket,local_depth)){
                            buckets[bucket_idx].second.clear();
                            int tmp = 1<<(local_depth);
                            tmp--;
                            for (int i = 0; i < tmp_bucket.size(); i++){
                                // m[tmp & (v[i])]++;
                                buckets[tmp & (tmp_bucket[i])].second.push_back(tmp_bucket[i]);
                                buckets[tmp & (tmp_bucket[i])].first = local_depth;
                                if(!isPresent(order,tmp & (tmp_bucket[i]))){
                                    order.push_back(tmp & (tmp_bucket[i]));
                                }
                            }
                            break;
                        }else{
                            order.push_back(add_one_to_msb(bucket_idx,local_depth-1));
                            buckets[add_one_to_msb(bucket_idx,local_depth-1)].first = local_depth;
                            cout<<"the local depth observed at "<<add_one_to_msb(bucket_idx,local_depth-1)<<" is: "<<local_depth<<endl;
                        }
                    }else if(local_depth==global_depth){
                        global_depth++;
                        buckets.resize(1<<global_depth,make_pair(0,vector<int> ()));
                    }else{
                        cout<<"local depth cant be greater than global\n";
                        break;
                    }
                }
                // cout<<"global depth: "<<global_depth<<endl;
                // for(int i=0;i<(1<<global_depth);i++){
                //     cout<<"local depth: "<<buckets[i].first<<endl;
                //     for(int j = 0;j<buckets[i].second.size();j++){
                //         cout<<buckets[i].second[j]<<" ";
                //     }
                //     cout<<endl;
                // }
            }
        }else if(input==3){
            cin>>value;
            // search the value in the extendible hash map
            int bucket_idx = 1<<(global_depth);
            bucket_idx--;
            bucket_idx &= value;
            while((buckets[bucket_idx].first)==0){
                bucket_idx = remove_msb(bucket_idx);
            }
            bool found = false;
            for(int i=0;i<buckets[bucket_idx].second.size();i++){
                if(buckets[bucket_idx].second[i]==value){
                    cout<<"Value ("<<value<<") found at "<<bucket_idx<<endl;
                    found = true;
                    break;
                }
            }
                if(!found){
                    cout<<"Value ("<<value<<") not found\n";
                }
        }else if(input==4){
            cin>>value;
            //delete the value from the hash map
            int bucket_idx = 1<<(global_depth);
            bucket_idx--;
            bucket_idx &= value;
            while((buckets[bucket_idx].first)==0){
                bucket_idx = remove_msb(bucket_idx);
            }
            bool found = false;
            auto found_itr = buckets[bucket_idx].second.begin();
            for(int i=0;i<buckets[bucket_idx].second.size();i++){
                if(buckets[bucket_idx].second[i]==value){
                    found = true;
                    break;
                }
                found_itr++;
            }
                if(!found){
                    cout<<"Value ("<<value<<") not found\nHence couldn't delete\n";
                }else{
                    buckets[bucket_idx].second.erase(found_itr);
                    cout<<"Value ("<<value<<") deleted\n";
                }
        }else if(input==5){
            // output the hash table
            cout<<"global depth: "<<global_depth<<endl;
            cout<<order.size()<<endl;
            for(int i=0;i<order.size();i++){
                cout<<buckets[order[i]].second.size()<<" "<<buckets[order[i]].first<<endl;
            }
            for(int i=0;i<(1<<global_depth);i++){
                cout<<"local depth: "<<buckets[i].first<<endl;
                for(int j = 0;j<buckets[i].second.size();j++){
                    cout<<buckets[i].second[j]<<" ";
                }
                cout<<endl;
            }
        }else if(input==6){
            break;
        }else{
            cout<<"You have entered an invalid input.\nThe program has been terminated\n";
            break;
        }
    }
}