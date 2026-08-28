#include<bits/stdc++.h>
#ifndef RAINSTOP_MODINT
#define RAINSTOP_MODINT
#include<chrono>
#include<utility>
#include<cstdint>
#include<type_traits>
#include<iostream>
template<uint64_t P> class ModInt;
template<uint64_t P> constexpr ModInt<P> qpw(ModInt<P>,uint64_t);
template<uint64_t P> constexpr ModInt<P> inv(ModInt<P>);
template<uint64_t P> constexpr ModInt<P> sqrt(ModInt<P>);
template<uint64_t P>
class ModInt{
    static_assert(!(P>>62));
    static_assert(P&1);
private:
    static constexpr uint64_t PP=P<<1;
    uint64_t x;
    static constexpr uint64_t calc_pinv(){
        uint64_t inv=P;
        return inv*=2-P*inv,inv*=2-P*inv,inv*=2-P*inv,inv*=2-P*inv,inv*=2-P*inv,-inv;
    }
    static constexpr uint64_t calc_r2(){
        __uint128_t r=-1;
        return r%P+1;
    }
    static constexpr uint64_t PINV=calc_pinv(),R2=calc_r2();
    static constexpr uint64_t reduce(__uint128_t T){
        uint64_t m=uint64_t(T)*PINV;
        uint64_t t=(T+__uint128_t(m)*P)>>64;
        return t;
    }
    struct RawConstructTag{};
    constexpr ModInt(uint64_t raw_val,RawConstructTag):x(raw_val){}
public:
    constexpr ModInt():x(0){}
    constexpr ModInt(uint64_t _x):x(reduce((__uint128_t)(_x)*R2)){}
    constexpr uint64_t val()const{uint64_t t=reduce(x);return t-(P&-(t>=P));}
    constexpr ModInt operator+(const ModInt&rhs)const{
        uint64_t t=x+rhs.x;
        return ModInt(t-=(PP&-(t>=PP)),RawConstructTag{});
    }
    constexpr ModInt operator-(const ModInt&rhs)const{
        uint64_t t=x-rhs.x;
        return ModInt(t+=(PP&(int64_t(t)>>63)),RawConstructTag{});
    }
    constexpr ModInt operator*(const ModInt&rhs)const{
        return ModInt(reduce(__uint128_t(x)*rhs.x),RawConstructTag{});
    }
    constexpr ModInt operator-()const{
        return ModInt((PP-x)&((int64_t)(-x)>>63),RawConstructTag{});
    }
    constexpr ModInt&operator+=(const ModInt&rhs){
        return x+=rhs.x,x-=(PP&-(x>=PP)),*this;
    }
    constexpr ModInt&operator-=(const ModInt&rhs){
        return x-=rhs.x,x+=(PP&(int64_t(x)>>63)),*this;
    }
    constexpr ModInt&operator*=(const ModInt&rhs){
        return x=reduce(__uint128_t(x)*rhs.x),*this;
    }
    constexpr bool operator==(const ModInt&rhs)const{
        uint64_t a=x-(P&(-(x>=P))),b=rhs.x-(P&(-(rhs.x>=P)));
        return a==b;
    }
    constexpr bool operator!=(const ModInt&rhs)const{
        uint64_t a=x-(P&(-(x>=P))),b=rhs.x-(P&(-(rhs.x>=P)));
        return a!=b;
    }
    friend constexpr ModInt<P> qpw<>(ModInt<P>,uint64_t);
    friend constexpr ModInt<P> inv<>(ModInt<P>);
    friend constexpr ModInt<P> sqrt<>(ModInt<P>);
};
template<uint64_t P>
constexpr ModInt<P> qpw(ModInt<P> x,uint64_t y){
    ModInt<P> res{1};
    for(;y;y>>=1,x*=x)
        if(y&1)
            res*=x;
    return res;
}
template<uint64_t P>
constexpr ModInt<P> inv(ModInt<P> x){
    ModInt<P> res{1};
    for(uint64_t y=P-2;y;y>>=1,x*=x)
        if(y&1)
            res*=x;
    return res;
}
template<uint64_t P>
constexpr ModInt<P> sqrt(ModInt<P>x){
    if(x==ModInt<P>(0))return x;
    ModInt<P> a{},w2;
    for(;qpw(w2=a*a-x,(P-1)>>1)==ModInt<P>(1);a+=ModInt<P>(1));
    if(w2==ModInt<P>(0))return a;
    uint64_t y=(P+1)>>1;
    std::pair<ModInt<P>,ModInt<P>>res{ModInt<P>(1),ModInt<P>()},X{a,ModInt<P>(1)};
    auto mul=[&w2](const std::pair<ModInt<P>,ModInt<P>>&a,const std::pair<ModInt<P>,ModInt<P>>&b) constexpr ->std::pair<ModInt<P>,ModInt<P>> {
        return {a.first*b.first+a.second*b.second*w2,a.first*b.second+a.second*b.first};
    };
    for(;y;y>>=1,X=mul(X,X))if(y&1)res=mul(res,X);
    return res.first;
}
template<> class ModInt<2>;
template<> constexpr ModInt<2> qpw(ModInt<2>,uint64_t);
template<> constexpr ModInt<2> inv(ModInt<2>);
template<> constexpr ModInt<2> sqrt(ModInt<2>);
template<>
class ModInt<2>{
private:
    uint8_t x;
public:
    constexpr ModInt<2>():x(0){}
    constexpr ModInt<2>(uint64_t _x):x(_x&1){}
    constexpr uint64_t val(){return x;}
    constexpr ModInt<2> operator+(const ModInt<2>&rhs)const{
        return ModInt<2>(x^rhs.x);
    }
    constexpr ModInt<2> operator-(const ModInt<2>&rhs)const{
        return ModInt<2>(x^rhs.x);
    }
    constexpr ModInt<2> operator*(const ModInt<2>&rhs)const{
        return ModInt<2>(x&rhs.x);
    }
    constexpr ModInt<2> operator-()const{
        return ModInt<2>(x);
    }
    constexpr ModInt<2>&operator+=(const ModInt<2>&rhs){
        return x^=rhs.x,*this;
    }
    constexpr ModInt<2>&operator-=(const ModInt<2>&rhs){
        return x^=rhs.x,*this;
    }
    constexpr ModInt<2>&operator*=(const ModInt<2>&rhs){
        return x&=rhs.x,*this;
    }
    constexpr bool operator==(const ModInt<2>&rhs)const{return x==rhs.x;}
    constexpr bool operator!=(const ModInt<2>&rhs)const{return x!=rhs.x;}
    constexpr friend ModInt<2> qpw<>(ModInt<2>,uint64_t);
    constexpr friend ModInt<2> inv<>(ModInt<2>);
    constexpr friend ModInt<2> sqrt<>(ModInt<2>);
};
template<>
constexpr ModInt<2> qpw(ModInt<2>x,uint64_t y){
    return ModInt<2>(x.x|(!y));
}
template<>
constexpr ModInt<2> inv(ModInt<2>x){
    return x;
}
template<>
constexpr ModInt<2> sqrt(ModInt<2>x){
    return x;
}
#endif
using namespace std;
namespace Mortis{
constexpr uint64_t P=1000000000000125953;
using mint=ModInt<P>;
struct Xorshift64s{
    uint64_t s;
    constexpr Xorshift64s(uint64_t seed=20100107):s(seed){}
    constexpr uint64_t operator()(){
        s^=s>>12,s^=s<<25,s^=s>>27;
        return s*0x2545F4914F6CDD1Dull;
    }
};
class info{
private:
    static constexpr mint calc_t(){
        Xorshift64s rng{};
        mint x;
        for(;x={uint64_t((__uint128_t(rng())*(P-1))>>64)+1},qpw(x,(P-1)>>1)==mint(1););
        return x;
    }
    static const mint t;
    static const mint invt;
    static inline int k=-1;
public:
    static void set_k(int _k){k=_k;}
    mint xp,xq;
    long long y;uint64_t cnt0;
private:
    info(mint _xp,mint _xq,long long _y,uint64_t _cnt0):xp(_xp),xq(_xq),y(_y),cnt0(_cnt0){}
public:
    info():xp(1),xq(1),y(0),cnt0(0){}
    info(mint _x,int _k):xp(_x),xq(1),y(0),cnt0(0){
        if(_x==mint{}){xp=mint(1),cnt0=1;}
        else{
            for(int i=0;i<_k;++i){
                if(qpw(xp,(P-1)>>1)==ModInt<P>(1))xp=sqrt(xp);
                else xp=sqrt(xp*invt),y|=1ull<<i;
            }y<<=k-_k;
        }
    }
    info operator*(const info&rsh)const{return info(xp*rsh.xp,xq*rsh.xq,y+rsh.y,cnt0+rsh.cnt0);}
    info operator/(const info&rsh)const{return info(xp*rsh.xq,xq*rsh.xp,y-rsh.y,cnt0-rsh.cnt0);}
    info&operator*=(const info&rsh){return xp*=rsh.xp,xq*=rsh.xq,y+=rsh.y,cnt0+=rsh.cnt0,*this;}
    info&operator/=(const info&rsh){return xp*=rsh.xq,xq*=rsh.xp,y-=rsh.y,cnt0-=rsh.cnt0,*this;}
    mint val(){
        if(cnt0)return mint{};
        if(y>0)return xp*qpw(xq,P-2)*qpw(t,y>>k);
        else if(y<0)return xp*qpw(xq,P-2)*qpw(invt,(-y)>>k);
        else return xp*qpw(xq,P-2);
    }
};
constexpr mint info::t=calc_t();
constexpr mint info::invt=inv(info::t);
void main(){
    int n,m,k;
    cin>>n>>m>>k,k=0;
    vector<vector<pair<int,uint64_t>>>a(m);
    for(auto&b:a){
        int d;cin>>d;
        k=max(k,d-1);
        b.resize(d);
        for(auto&[s,_]:b)cin>>s;
        for(auto&[_,x]:b)cin>>x;
    }
    vector<info>f(1<<n);
    info::set_k(k);
    int delta=0;
    for(auto&b:a){
        int d=b.size()-1;
        int u=b[d].first;mint v=b[d].second;
        delta^=u;
        vector<int>mask(1<<d);
        vector<mint>g(1<<d);
        for(int i=0;i<d;++i)tie(mask[1<<i],g[1<<i])=b[i],mask[1<<i]^=u;
        for(int s=1;s<(1<<d);++s)mask[s]=mask[s^(s&(-s))]^mask[s&(-s)];
        for(int i=0;i<d;++i)
            for(int s=0;s<(1<<d);++s)
                if((s>>i)&1){
                    int t=s^(1<<i);
                    auto p=g[t],q=g[s];
                    g[t]=p+q,g[s]=p-q;
                }
        for(int s=0;s<(1<<d);++s)g[s]+=v;
        vector<info>h(1<<d);
        for(int s=0;s<(1<<d);++s)h[s]=info(g[s],d);
        for(int i=0;i<d;++i)
            for(int s=0;s<(1<<d);++s)
                if((s>>i)&1){
                    int t=s^(1<<i);
                    auto p=h[t],q=h[s];
                    h[t]=p*q,h[s]=p/q;
                }
        for(int s=0;s<(1<<d);++s)f[mask[s]]*=h[s];
    }
    for(int i=0;i<n;++i)
        for(int s=0;s<(1<<n);++s)
            if((s>>i)&1){
                int t=s^(1<<i);
                auto p=f[t],q=f[s];
                f[t]=p*q,f[s]=p/q;
            }
    vector<mint>res(1<<n);
    for(int s=0;s<(1<<n);++s)res[s]=f[s].val();
    for(int i=0;i<n;++i)
        for(int s=0;s<(1<<n);++s)
            if((s>>i)&1){
                int t=s^(1<<i);
                auto p=res[t],q=res[s];
                res[t]=(p+q)*mint((P+1)>>1),res[s]=(p-q)*mint((P+1)>>1);
            }
    for(int s=0;s<(1<<n);++s)cout<<res[s^delta].val()<<' ';
}
}
int main(){
    ios_base::sync_with_stdio(false),cin.tie(nullptr);
    Mortis::main();
    return 0;
}
