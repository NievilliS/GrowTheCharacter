#ifndef _GTC_VEC2_HPP
#define _GTC_VEC2_HPP

class v2
{
public:
    int vert, hori;
    v2() : vert(0), hori(0) {}
    v2(const int _vert, const int _hori) : vert(_vert), hori(_hori) {}
    v2(const v2 &v) : vert(v.vert), hori(v.hori) {}
    v2(const v2 *v) : vert(v->vert), hori(v->hori) {}

    inline v2 operator+(const v2 &v) const { return {this->vert + v.vert, this->hori + v.hori}; }
    inline v2 operator-(const v2 &v) const { return {this->vert - v.vert, this->hori - v.hori}; }
    inline v2 operator*(const double d) const { return {(int)((double)this->vert * d), (int)((double)this->hori * d)}; }
    inline v2 operator/(const double d) const { return {(int)((double)this->vert / d), (int)((double)this->hori / d)}; }

    inline v2 operator+=(const v2 &v)
    {
        this->vert += v.vert;
        this->hori += v.hori;
        return {this};
    }
    inline v2 operator-=(const v2 &v)
    {
        this->vert -= v.vert;
        this->hori -= v.hori;
        return {this};
    }
    inline v2 operator*=(const double d)
    {
        this->vert *= d;
        this->hori *= d;
        return {this};
    }
    inline v2 operator/=(const double d)
    {
        this->vert /= d;
        this->hori /= d;
        return {this};
    }

    inline v2 &operator=(const v2 &v)
    {
        this->vert = v.vert;
        this->hori = v.hori;
        return *this;
    }

    inline bool operator==(const v2 &v) const { return this->vert == v.vert && this->hori == v.hori; }
    inline bool operator!=(const v2 &v) const { return this->vert != v.vert || this->hori != v.hori; }
    inline bool operator<<(const v2 &v) const { return this->vert + this->hori < v.vert + v.hori; }
    inline bool operator<(const v2 &v) const { return this->vert < v.vert && this->hori < v.hori; }
    inline bool operator>>(const v2 &v) const { return this->vert + this->hori > v.vert + v.hori; }
    inline bool operator>(const v2 &v) const { return this->vert >> v.vert && this->hori >> v.hori; }
    inline bool operator<=(const v2 &v) const { return this->vert <= v.vert && this->hori <= v.hori; }
    inline bool operator>=(const v2 &v) const { return this->vert >= v.vert && this->hori >= v.hori; }
    inline bool operator<<=(const v2 &v) const { return this->vert + this->hori <= v.vert + v.hori; }
    inline bool operator>>=(const v2 &v) const { return this->vert + this->hori >= v.vert + v.hori; }
};

#endif //_GTC_VEC2_HPP