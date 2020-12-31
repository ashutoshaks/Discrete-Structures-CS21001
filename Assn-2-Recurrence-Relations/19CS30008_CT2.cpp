// Ashutosh Kumar Singh
// 19CS30008

#include <iostream>
#include <cstdlib>
#include <math.h>
using namespace std;

#define ll long long

typedef struct 
{
	ll a, b, c, d;
} splnum;

ll gcd(ll a, ll b)
{
	return (a ? gcd(b % a, a) : b);
}

// normalises a special number by dividing a, b, c by gcd(a, b, c)
void normalise(splnum &u)
{
	ll hcf = gcd(u.a, gcd(u.b, u.c));
	if(hcf != 1)
	{
		u.a /= hcf;
		u.b /= hcf;
		u.c /= hcf;
	}
	// ensuring that c remains positive
	if(u.c < 0)
	{
		u.a *= -1;
		u.b *= -1;
		u.c *= -1;
	}
}

// checks if a special number is zero
bool iszero(splnum u)
{
	return (u.a == 0 && u.b == 0 && u.d == 0);
}

// checks if a special number is one
bool isone(splnum u)
{
	return (u.a == 1 && u.b == 0 && u.c == 1 && u.d == 0);
}

// checks if two special numbers are equal
bool isequal(splnum u, splnum v)
{
	return (u.a == v.a && u.b == v.b && u.c == v.c && u.d == v.d);
}

// prlls a special number as per the specified format
void prnspl(splnum u)
{
	if(iszero(u))
	{
		cout << 0;
		return;
	}
	if(u.d == 0)
	{
		if(u.a == 0)
			return;
		cout << u.a;
		if(u.c != 1)
			cout << " / " << u.c;
	}
	else
	{
		cout << "(";
		if(u.a == 0)
		{
			if(u.b < 0)
			{
				if(u.b == -1)
					cout << "-";
				else
					cout << "- " << -u.b << " ";
			}
			else
			{
				if(u.b > 1)
					cout << u.b << " ";
			}

		}
		else
		{
			cout << u.a;
			if(u.b > 0)
			{
				if(u.b == 1)
					cout << " + ";
				else
					cout << " + " << u.b << " ";
			}
			else
			{
				if(u.b == -1)
					cout << " - ";
				else
					cout << " - " << -u.b << " ";
			}
		}
		cout << "sqrt(" << u.d << "))";
		if(u.c != 1)
			cout << " / " << u.c;
	}
}

// returns the negative of a special number
splnum neg(splnum u)
{
	splnum ans = {-u.a, -u.b, u.c, u.d};
	return ans;
}

// returns the sum of two special numbers (u + v)
splnum addspl(splnum u, splnum v)
{
	splnum ans;
	if((u.d != v.d) && (u.d != 0 && v.d != 0))
	{
		cout << "Trying to add "; prnspl(u); cout << " and "; prnspl(v);
		cout << "\nCannot add two special numbers with different irrational parts.\n";
		exit(0);
	}
	
	ans.a = u.a * v.c + u.c * v.a;
	ans.b = u.b * v.c + u.c * v.b;
	ans.c = u.c * v.c;
	ans.d = (v.d ? v.d : u.d);
	
	normalise(ans);
	return ans;
}

// returns the difference of two special numbers (u - v)
splnum subspl(splnum u, splnum v)
{
	if(isequal(u, v))
	{
		splnum ans = {0, 0, 1, 0}; // if u and v are zero, return 0
		return ans;
	}
	if((u.d != v.d) && (u.d != 0 && v.d != 0))
	{
		cout << "Trying to subtract "; prnspl(u); cout << " and "; prnspl(v);
		cout << "\nCannot subtract two special numbers with different irrational parts.\n";
		exit(0);
	}
	
	return addspl(u, neg(v));
}

// returns the product of two special numbers (u * v)
splnum mulspl(splnum u, splnum v)
{
	splnum ans;
	if(iszero(u) || iszero(v)) // if any one of u or v is zero, return 0
	{
		ans = {0, 0, 1, 0};
		return ans;
	}
	if((u.d != v.d) && (u.d != 0 && v.d != 0))
	{
		cout << "Trying to multiply "; prnspl(u); cout << " and "; prnspl(v);
		cout << "\nCannot multiply two special numbers with different irrational parts.\n";
		exit(0);
	}

	ans.a = u.a * v.a + u.b * v.b * u.d;
	ans.b = u.a * v.b + u.b * v.a;
	ans.c = u.c * v.c;
	ans.d = (u.d ? u.d : v.d);
	
	normalise(ans);
	return ans;
}

// returns the inverse of a special number (1 / u), provided u is not zero
splnum invspl(splnum u)
{
	if(iszero(u))
	{
		cout << "Inverse of 0 cannot be found.\n";
		exit(0);
	}
	splnum ans;
	
	ans.a = u.c * u.a;
	ans.b = -u.c * u.b;
	ans.c = u.a * u.a - u.b * u.b * u.d;
	ans.d = u.d;
	
	normalise(ans);
	return ans;
}

// returns a special number after dividing two special numbers (u / v), provided v is not zero
splnum divspl(splnum u, splnum v)
{
	if(iszero(v))
	{
		cout << "Division by 0 is not possible.\n";
		exit(0);
	}
	splnum v1 = invspl(v);
	
	if((u.d != v1.d) && (u.d != 0 && v1.d != 0))
	{
		cout << "Trying to divide "; prnspl(u); cout << " by "; prnspl(v);
		cout << "\nCannot divide two special numbers with different irrational parts.\n";
		exit(0);
	}
	return mulspl(u, v1);
}

// checks if the number n is a perfect square or not
// it uses the fact that all powers of primes must be even
bool isPerfectSquare(ll n)
{
	if(n == 0)
		return 1;
	if(n < 0)
		return 0;
	ll cnt = 0, i;
	while(n % 2 == 0)
	{
		cnt++;
		n /= 2;
	}
	if(cnt % 2)
		return 0;
	for(i = 3; i * i <= n; i += 2)
	{
		cnt = 0;
		while(n % i == 0)
		{
			n /= i;
			cnt++;
		}
		if(cnt % 2)
			return 0;
	}
	if(n > 1)
		return 0;
	return 1;
}

// finds the two roots of the quadratic equation x^2 - r * x - s = 0 and stores them in roots[]
void findroots(ll r, ll s, splnum roots[2])
{
	ll disc = r * r + 4 * s, r1, r2;
	
	if(disc == 0 || isPerfectSquare(disc))
	{
		disc = sqrt(disc);
		r1 = r + disc;
		r2 = r - disc;
		roots[0].d = roots[1].d = 0;
		roots[0].b = roots[1].b = 0;
		if(r1 % 2 == 0 && r2 % 2 == 0)
		{
			roots[0].c = roots[1].c = 1;
			roots[0].a = r1 / 2;
			roots[1].a = r2 / 2;
		}
		else
		{
			roots[0].c = roots[1].c = 2;
			roots[0].a = r1;
			roots[1].a = r2;
		}
	}
	else
	{
		if(r % 2 == 0 && disc % 4 == 0)
		{
			r /= 2;
			disc /= 4;
			roots[0].c = roots[1].c = 1;
		}
		else
		{
			roots[0].c = roots[1].c = 2;
		}
		roots[0].a = roots[1].a = r;
		roots[0].d = roots[1].d = disc;
		roots[0].b = 1;
		roots[1].b = -1;
	}
}

// this function finds the constants c1 and c2 such that : 
// a(n) = (c1) * (p1) ^ n + (c2) * (p2) ^ n, and
// a(0) = sa0 and a(1) = sa1
void findconstants(splnum p1, splnum p2, splnum sa0, splnum sa1, splnum &c1, splnum &c2)
{
	c1 = subspl(sa1, mulspl(sa0, p2));
	c1 = divspl(c1, subspl(p1, p2));
	c2 = subspl(mulspl(sa0, p1), sa1);
	c2 = divspl(c2, subspl(p1, p2));
}

// this function solves the homogeneous part of the recurrence
void solvehomogeneous(ll r, ll s, ll a0, ll a1)
{
	splnum roots[2], p1, p2, c1, c2, sa0, sa1;
	sa0 = {a0, 0, 1, 0};
	sa1 = {a1, 0, 1, 0};

	findroots(r, s, roots);
	p1 = roots[0];
	p2 = roots[1];
	cout << "Homogeneous solution :\n";
	
	if(!isequal(p1, p2))
	{
		findconstants(p1, p2, sa0, sa1, c1, c2);
		cout << "["; prnspl(c1); cout << "] [";
		prnspl(p1); cout << "]^n + ["; prnspl(c2);
		cout << "] ["; prnspl(p2); cout << "]^n\n";
	}
	else
	{
		c1 = sa0;
		c2 = subspl(divspl(sa1, p1), sa0);
		cout << "[("; prnspl(c2); cout << ")n + (";
		prnspl(c1); cout << ")] ["; prnspl(p1);
		cout << "]^n\n";
	}
}

// this function first finds the particular solution and then using that finds the homogeneous solution
void solvenonhomogeneous(ll r, ll s, ll t, ll a0, ll a1)
{
	splnum psol, roots[2], p1, p2, c1, c2, sa0, sa1;
	sa0 = {a0, 0, 1, 0};
	sa1 = {a1, 0, 1, 0};

	findroots(r, s, roots);
	p1 = roots[0];
	p2 = roots[1];	
	
	ll multiplicity = isone(p1) + isone(p2);

	if(multiplicity == 0 && !isequal(p1, p2))
	{
		psol = {t, 0, 1 - r - s, 0};
		normalise(psol);

		cout << "Particular solution : "; prnspl(psol); cout << endl;
		cout << "Homogeneous solution :\n";

		sa0 = subspl(sa0, psol);
		sa1 = subspl(sa1, psol);
		
		findconstants(p1, p2, sa0, sa1, c1, c2);
		cout << "["; prnspl(c1); cout << "] [";
		prnspl(p1); cout << "]^n + ["; prnspl(c2);
		cout << "] ["; prnspl(p2); cout << "]^n\n";
	}
	else if(multiplicity == 0 && isequal(p1, p2))
	{
		psol = {t, 0, 1 - r - s, 0};
		normalise(psol);

		cout << "Particular solution : "; prnspl(psol); cout << endl;
		cout << "Homogeneous solution :\n";

		sa0 = subspl(sa0, psol);
		sa1 = subspl(sa1, psol);
		c1 = sa0;
		c2 = subspl(divspl(sa1, p1), sa0);
		cout << "[("; prnspl(c2); cout << ")n + (";
		prnspl(c1); cout << ")] ["; prnspl(p1);
		cout << "]^n\n";
	}
	else if(multiplicity == 1 && !isequal(p1, p2))
	{
		psol = {t, 0, r + 2 * s, 0};
		normalise(psol);

		cout << "Particular solution : ["; prnspl(psol); cout << "] n\n";
		cout << "Homogeneous solution :\n";

		sa1 = subspl(sa1, psol);

		findconstants(p1, p2, sa0, sa1, c1, c2);
		cout << "["; prnspl(c1); cout << "] [";
		prnspl(p1); cout << "]^n + ["; prnspl(c2);
		cout << "] ["; prnspl(p2); cout << "]^n\n";
	}
	else if(multiplicity == 2 && isequal(p1, p2))
	{
		psol = {-t, 0, r + 4 * s, 0};
		normalise(psol);

		cout << "Particular solution : ["; prnspl(psol); cout << "] n^2\n";
		cout << "Homogeneous solution :\n";

		sa1 = subspl(sa1, psol);
		c1 = sa0;
		c2 = subspl(divspl(sa1, p1), sa0);
		cout << "[("; prnspl(c2); cout << ")n + (";
		prnspl(c1); cout << ")] ["; prnspl(p1);
		cout << "]^n\n";
	}
}

int main()
{
	ll r, s, t, a0, a1;
	splnum roots[2];
	cout << "r = "; cin >> r;
	cout << "s = "; cin >> s; 
	cout << "t = "; cin >> t;
	cout << "a0 = "; cin >> a0;
	cout << "a1 = "; cin >> a1;

	cout << "\n+++ Solving the homogeneous recurrence\n";
	cout << "Characteristic equation: x^2 + (" << -r << ")x + (" << -s << ") = 0\n";
	findroots(r, s, roots);
	cout << "Root 1 = "; prnspl(roots[0]); cout << endl;
	cout << "Root 2 = "; prnspl(roots[1]); cout << endl;
	solvehomogeneous(r, s, a0, a1);

	cout << "\n+++ Solving the nonhomogeneous recurrence\n";
	cout << "Characteristic equation: x^2 + (" << -r << ")x + (" << -s << ") = 0\n";
	findroots(r, s, roots);
	cout << "Root 1 = "; prnspl(roots[0]); cout << endl;
	cout << "Root 2 = "; prnspl(roots[1]); cout << endl;
	solvenonhomogeneous(r, s, t, a0, a1);
}