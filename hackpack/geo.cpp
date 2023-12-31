// Templated point/vector impl for any numeric type.
template<typename n_t>
struct pt {
    n_t x, y;

    pt() : pt(0, 0) {}

    pt(n_t x, n_t y) : x(x), y(y) {
        static_assert(std::is_arithmetic<n_t>::value, "value type must be numeric");
    }

    static pt polar(n_t theta, n_t r) {
        return {r * cos(theta), r * sin(theta)};
    }

    friend std::istream &operator>>(std::istream &in, pt<n_t> &rhs) {
        in >> rhs.x >> rhs.y;
        return in;
    }

    friend std::ostream &operator<<(std::ostream &out, const pt<n_t> &rhs) {
        out << rhs.x << " " << rhs.y;
        return out;
    }

    pt operator+(const pt &r) const {
        return {x + r.x, y + r.y};
    }

    pt operator-(const pt &r) const {
        return {x - r.x, y - r.y};
    }

    pt operator*(n_t r) const {
        return {x * r, y * r};
    }

    pt operator/(n_t r) const {
        return {x / r, y / r};
    }

    bool operator==(const pt &u) const {
        return x == u.x && y == u.y;
    }

    bool operator<(const pt &u) const {
        if (x != u.x) return x < u.x;
        return y < u.y;
    }

    n_t distance(pt u) const {
        return sqrt(std::pow(x - u.x, 2) + std::pow(y - u.y, 2));
    }

    n_t distance2(pt u) const {
        return std::pow(x - u.x, 2) + std::pow(y - u.y, 2);
    }

    n_t dot(pt u) const {
        return x * u.x + y * u.y;
    }

    n_t cross(pt u) const {
        return x * u.y - y * u.x;
    }

    // Computes the cross product of (u - this vector) and (v - this vector), such as for area
    n_t cross(pt u, pt v) const {
        return (u - *this).cross(v - *this);
    }

    n_t norm() const {
        return x * x + y * y;
    }

    n_t mag() const {
        return std::sqrt(norm());
    }

    n_t theta() const {
        return atan2(y, x);
    }

    n_t angle(pt u) {
        return (u - *this).theta();
    }

    pt unit() const {
        n_t m = mag();
        return {x / m, y / m};
    }

    pt midpoint(pt u) const {
        return *this + (u - *this) / 2.0f;
    }

    // Projects the vector u onto this vector
    pt project(pt u) const {
        return unit() * dot(u) / mag();
    }

    // Gets the unit vectors perpendicular to this vector
    std::pair <pt, pt> perp() const {
        n_t m = mag();
        return {{y / m,  -x / m},
                {-y / m, x / m}};
    }
};

template<typename n_t>
struct seg {
    pt<n_t> a, b;

    seg(const pt<n_t> &a, const pt<n_t> &b) : a(a), b(b) {}

    static seg from(const pt<n_t> &start, const pt<n_t> &dir) {
        return seg(start, start + dir);
    }

    pt<n_t> dir() const {
        return b - a;
    }

    n_t distance(pt<n_t> v) const {
        auto u = dir();
        return std::abs(u.cross(v) / u.mag());
    }


    // Returns true if the segments intersect, and sets out to the intersection point
    // https://stackoverflow.com/a/565282
    bool intersection(const seg &o, pt<n_t> &out) const {
        const pt<n_t> &p = a, &q = o.a;
        pt<n_t> r = dir(), s = o.dir();

        // If the segments intersect, there exist some t,u such that p + tr = q + us
        n_t t = (q - p).cross(s) / r.cross(s);
        n_t u = (q - p).cross(r) / r.cross(s);

        if (r.cross(s) == 0) {
            if ((q - p).cross(r) == 0) {
                // Collinear
                n_t t0 = (q - p).cross(r) / r.norm();
                n_t t1 = t0 + s.dot(r) / r.norm();
                if (t0 > t1) swap(t0, t1);

                // TODO: Pick more consistent intersection points in collinear cases
                if (t0 >= 0 && t1 <= 0) {
                    // Collinear, intersecting at [t0, t1]
                    out = p + r * t0;
                    return true;
                }
                if (t0 <= 0 && t1 >= 0) {
                    // Collinear, intersecting at [0, min(1, t1)]
                    out = p + r * min((n_t) 1, t1);
                    return true;
                }
                if (t0 <= 1 && t1 >= 1) {
                    // Collinear, intersecting at [max(0, t0), 1]
                    out = p + r * max((n_t) 0, t0);
                    return true;
                }

                // Collinear, not intersecting
                return false;
            }
            // Parallel, not intersecting
            return false;
        }

        if (0 <= t && t <= 1 && 0 <= u && u <= 1) {
            // Not parallel, intersecting
            out = p + r * t;
            return true;
        }

        // Not parallel, not intersecting
        return false;
    }
};

// Returns the absolute area of the triangle formed by the points a, b and c
template<typename n_t>
n_t area(const pt<n_t> &a, const pt<n_t> &b, const pt<n_t> &c) {
    pt<n_t> u = b - a, v = c - a;
    return std::abs(u.cross(v)) / (n_t) 2;
}

// Finds the midpoints of both possible circles with diameter d whose circumferences contain the points u and v
// Assumes that the points are *not* at a distance of d, in which case there is only one circle (check this yourself)
template<typename n_t>
std::pair <pt<n_t>, pt<n_t>> find_circles(pt<n_t> u, pt<n_t> v, n_t d, n_t eps = 1e-6) {
    pt<n_t> mid = u.midpoint(v), dif = v - u;
    n_t dist = u.distance(v);

    std::pair <pt<n_t>, pt<n_t>> perp = dif.perp();
    double h = std::sqrt(d * d / 4 - dist * dist / 4);
    return {
            mid + perp.first * h,
            mid + perp.second * h
    };
}

// Finds the midpoint and radius of a circle whose circumference contains the points a, b and c
template<typename n_t>
bool find_circle(pt<n_t> a, pt<n_t> b, pt<n_t> c, std::pair <pt<n_t>, n_t> &out, n_t ray_length = 1e5) {
    pt<n_t> u = b - a, v = c - a;
    if (u.cross(v) < 0) swap(u, v);

    pt<n_t> m_u = u / 2, m_v = v / 2;
    seg<n_t> h_u = seg<n_t>::from(m_u, u.perp().second * ray_length);
    seg<n_t> h_v = seg<n_t>::from(m_v, v.perp().first * ray_length);

    pt<n_t> mid;
    if (!h_u.intersection(h_v, mid)) return false;

    out = {mid, mid.distance(a)};
    return true;
}

// Constructs a convex hull with the given points, returning them in counterclockwise order.
template<typename n_t>
std::vector <pt<n_t>> convex(std::vector <pt<n_t>> pts, double eps = 1e-9) {
    if (pts.size() <= 1) return pts;

    std::sort(pts.begin(), pts.end());
    std::vector <pt<n_t>> hull(pts.size() + 1);

    int s = 0, hull_size = 0;
    for (int i = 2; i--; s = --hull_size, std::reverse(pts.begin(), pts.end())) {
        for (auto &p: pts) {
            while (hull_size >= s + 2 && hull[hull_size - 2].cross(hull[hull_size - 1], p) <= 0 + eps) hull_size--;
            hull[hull_size++] = p;
        }
    }

    return {
            hull.begin(),
            hull.begin() + hull_size - (hull_size == 2 && hull[0] == hull[1])
    };
}

// Finds all antipodal pairs for the given hull with rotating calipers, assuming points in counterclockwise order.
// Returns the biggest distance between any antipodal pair, or 0 if there are less than two points.
template<typename n_t>
n_t max_distance(const std::vector <pt<n_t>> &hull) {
    if (hull.size() <= 1) return (n_t) 0;
    if (hull.size() == 2) return hull[0].distance(hull[1]);

    int n = hull.size();
    int j = n >= 2;
    n_t max_d2 = 0;

    rep(i, 0, j)
    {
        for (;; j = (j + 1) % n) {
            max_d2 = max(max_d2, hull[i].distance2(hull[j]));
            if ((hull[(j + 1) % n] - hull[j]).cross(hull[i + 1] - hull[i]) >= 0) break;
        }
    }

    return std::sqrt(max_d2);
}

// TODO: polygon; point in polygon (above/below cross with all segments that intersect)