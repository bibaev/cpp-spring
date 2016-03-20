#include <iostream>
#include <vector>
#include <memory>

struct shape {
public:
    virtual ~shape()
    {
        std::cout << "virtual ~shape call." << std::endl;
    }

    virtual void intersect(shape& other) = 0;
    virtual void intersect_impl(struct point&) = 0;
    virtual void intersect_impl(struct rectangle&) = 0;
    virtual void intersect_impl(struct circle&) = 0;
};

struct point : shape
{
    virtual ~point()
    {
    }

    point(double x, double y) : x_(x), y_(y){}

    void intersect(shape& other) override
    { other.intersect_impl(*this); };

    void intersect_impl(point&) override;
    void intersect_impl(rectangle&) override;
    void intersect_impl(circle&) override;
private:
    double x_;
    double y_;
};

void point::intersect_impl(point&)
{
    std::cout << "Intersect point and point." << std::endl;
}

void point::intersect_impl(rectangle&)
{
    std::cout << "Intersect point and rectangle." << std::endl;
}

void point::intersect_impl(circle&)
{
    std::cout << "Intersect point and circle." << std::endl;
}

struct rectangle : shape
{
    virtual ~rectangle()
    {
    }

    rectangle(point const& top_left, point const& bottom_right) : top_left_(top_left), bottom_right_(bottom_right){}

    void intersect(shape& other) override
    {
        other.intersect_impl(*this);
    };

    void intersect_impl(point&) override;
    void intersect_impl(rectangle&) override;
    void intersect_impl(circle&) override;
private:
    point top_left_;
    point bottom_right_;

};

void rectangle::intersect_impl(point&)
{
    std::cout << "Intersect rectangle and point." << std::endl;
}

void rectangle::intersect_impl(rectangle&)
{
    std::cout << "Intersect rectangle and rectangle." << std::endl;
}

void rectangle::intersect_impl(circle&)
{
    std::cout << "Intersect rectangle and circle." << std::endl;
}

template<typename T, typename U>
void intersect(T& left, U& right)
{
    left.intersect(right);
}

struct circle : shape
{
    virtual ~circle()
    {
    }

    circle(point const& center, double radius)
        : center_(center), radius_(radius)
    {}

    void intersect(shape& other) override
    {
        other.intersect_impl(*this);
    };
    void intersect_impl(point &other) override;
    void intersect_impl(rectangle &other) override;
    void intersect_impl(circle &other) override;
private:
    point center_;
    double radius_;
};

void circle::intersect_impl(point& other)
{
    std::cout << "Intersect circle and point." << std::endl;
}

void circle::intersect_impl(rectangle& other)
{
    std::cout << "Intersect circle and rectangle." << std::endl;
}

void circle::intersect_impl(circle& other)
{
    std::cout << "Intersect circle and circle." << std::endl;
}


void virt_func_tests() {
    std::vector<std::shared_ptr<shape>> shapes = { std::make_shared<point>(1, 2)
        , std::make_shared<rectangle>(point(0, 10), point(10, 0))
        , std::make_shared<circle>(point(10, 0), 10.) };
    for (auto& shape1 : shapes) {
        for (auto& shape2 : shapes) {
            intersect(*shape1, *shape2);
        }
    }
}

//int main() {
//    virt_func_tests();
//}
