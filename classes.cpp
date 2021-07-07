
class Parent
{
    int aa;

    Parent(int a) : aa(a)
    {
    }
};

class Child : public Parent
{
    Parent parentti;
};

int main()
{

    return 0;
}
