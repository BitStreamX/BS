/*
 * 判断是否是素数
*/
bool isPrime(unsigned long n){
    if(n<=3){
        return n>1;
    }
    else if(n%2==0||n%3==0){
        return false;
    }
    else{
        for(unsigned short i=5;i*i<=n;i+=6){
            if(n%i==0||n%(i+2)==0){
                return false;
            }
        }

        return true;
    }
}

/*
 * 指定概率随机数测试
 */
void randPblTst(){
    using std::cout;
    using std::endl;
    
    srand(unsigned(time(0)));
    
    const int BASE=10000;
    const int CNT=10;
    double pbl[CNT]={0.3,0.25,0.15,0.09,0.06,0.05,0.04,0.03,0.02,0.01};
    
    
    int data[CNT]={};
    for(int idx=0;idx<BASE;++idx){
        int i=randPbl(CNT,pbl,BASE);
        ++data[i];
    }
    
    for(int idx=0;idx<CNT;++idx){
        cout << (idx+1) << ":" << double(data[idx])/BASE << endl;
    }
}

/*
 * 生成指定概率的随机数
 */
int randPbl(int cnt,double * pbl,int base){
    int idx=0;
    double p=0;
    
    
    int rdm=random(0,base-1)%base;
    
    for(idx=0;idx<cnt;++idx){
        p+=pbl[idx];
        
        if(rdm<base*p){
            break;
        }
    }
    
    return idx;
}
