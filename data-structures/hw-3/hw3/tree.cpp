// tree.cpp
// Dhruv Chakraborty, UID: 204-962-098
// UCLA CS32, HW3: part 4

int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0)
		return 1;
	if (n1 <= 0)
		return 0;

	if (a1[0] == a2[0])
		return countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1) + countIncludes(a1 + 1, n1 - 1, a2, n2);
	else
		return countIncludes(a1 + 1, n1 - 1, a2, n2);
}

void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

void divide(double a[], int n, double divider,
				    int& firstNotGreater, int& firstLess)
{
    if (n < 0)
	n = 0;

      // It will always be the case that just before evaluating the loop
      // condition:
      //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
      //  Every element earlier than position firstNotGreater is > divider
      //  Every element from position firstNotGreater to firstUnknown-1 is
      //    == divider
      //  Every element from firstUnknown to firstLess-1 is not known yet
      //  Every element at position firstLess or later is < divider

    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
	if (a[firstUnknown] < divider)
	{
	    firstLess--;
	    exchange(a[firstUnknown], a[firstLess]);
	}
	else
	{
	    if (a[firstUnknown] > divider)
	    {
		exchange(a[firstNotGreater], a[firstUnknown]);
		firstNotGreater++;
	    }
	    firstUnknown++;
	}
    }
}

void order(double a[], int n)
{
	if (n <= 1)
		return;

	int firstNotGreater, firstLess;

	divide(a, n, a[0], firstNotGreater, firstLess);

	order(a, firstNotGreater - 1); // orders left
	order(a + firstLess, n - firstLess); // orders right
}