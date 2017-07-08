#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

/**
 *	This block for operations with the common module
 */

ulong modularAdd(ulong a, ulong b)
{
	return ((a % moduleStruct.module) + (b % moduleStruct.module)) % moduleStruct.module;
}

ulong modularSub(ulong a, ulong b)
{
	return modularAdd(a, modularInverseAdd(b));
}


ulong modularDiv(ulong a, ulong b)
{
	ulong a_new = a % moduleStruct.module;
	ulong b_new = b % moduleStruct.module;
	return modularMult(a_new, modularInverseMult(b_new));
}

//	Fast multiplication uses The Chinese theorem on residues
ulong modularMult(ulong a, ulong b) {
	// if (a < ((ulong)1 << 32) && b < ((ulong)1 << 32)) {
	// 	return littleModularMultUniver(a, b, moduleStruct.module);
	// }
	if (a % moduleStruct.module == 0 || b % moduleStruct.module == 0) {
		return 0;
	}
	if (moduleStruct.module < ((ulong)1 << 32)) {
		return littleModularMultUniver(a, b, moduleStruct.module);
	}
	a %= moduleStruct.module;
	b %= moduleStruct.module;
	ulong r = 0;
	ulong res = 0;
	for (int i = 0; i < moduleStruct.masSize; i++) {
		r = littleModularMultUniver(a, b, moduleStruct.partsOfModule[i]);
		res = modularAdd(\
		                 res, \
		                 littleModularMultUniver(\
		                         littleModularMultUniver(r, moduleStruct.partsOfModule[i + moduleStruct.masSize * 2], moduleStruct.partsOfModule[i]), \
		                         moduleStruct.partsOfModule[i + moduleStruct.masSize * 1], \
		                         moduleStruct.module\
		                                        )\
		                );
	}
	return res;
}

//быстрое возведение в степень для модуля>2^32
ulong modularDeg(ulong a, ulong k)
{
	a %= moduleStruct.module;
	ulong res = 1;
	while (k > 0)
	{
		if (k % (ulong)2 == (ulong)1)
		{
			k--;
			res = modularMult(res, a);

		}
		k /= (ulong)2;
		a = modularMult(a, a);

	}
	return res;
}

ulong modularInverseAdd(ulong a)
{
	return moduleStruct.module - (a % moduleStruct.module);
}

//быстрый поиск обратного по умножению для модуля>2^32
ulong modularInverseMult(ulong a)
{
	return modularDeg(a, euler(moduleStruct.module) - 1);
}

void modularMatrixMult(ulong *mat1, ulong *mat2, ulong *result, ulong lines) {
	ulong i, j, k;
	for (i = 0; i < lines; i++) {
		for (k = 0; k < lines; k++) {
			result[i * lines + k] = 0;
			for (j = 0; j < lines; j++) {
				result[i * lines + k] = modularAdd(result[i * lines + k], modularMult(mat1[i * lines + j], mat2[j * lines + k]));
			}
		}
	}
}

/**
 *	This block for operations with an any module
 */

ulong modularAddUniver(ulong a, ulong b, ulong module)
{
	return ((a % module) + (b % module)) % module;
}

ulong modularSubUniver(ulong a, ulong b, ulong module)
{
	return modularAddUniver(a, modularInverseAddUniver(b, module), module);
}

ulong modularDivUniver(ulong a, ulong b, ulong module)
{
	ulong a_new = a % module;
	ulong b_new = b % module;
	return modularMultUniver(a_new, modularInverseMultUniver(b_new, module), module);
}

// multiplication for numbers < 2^32
ulong littleModularMultUniver(ulong a, ulong b, ulong littleModule) {
	return (((ulong)a % (ulong)littleModule) * ((ulong)b % (ulong)littleModule)) % (ulong)littleModule;
}

ulong modularMultUniver(ulong a, ulong b, ulong module)
{
	if (module < ((ulong)1 << 32)) {
		return littleModularMultUniver(a, b, module);
	}
	ulong res = 0;
	while (b > 0)
	{
		if (b % (ulong)2 == (ulong)1)
		{
			b--;
			res = modularAddUniver(res, a, module);

		}
		a = modularAddUniver(a, a, module);
		b /= (ulong)2;

	}
	return res;
}

ulong modularDegUniver(ulong a, ulong k, ulong module)
{
	ulong res = 1;
	while (k > 0)
	{
		if (k % (ulong)2 == (ulong)1)
		{
			k--;
			res = modularMultUniver(res, a, module);

		}
		k /= (ulong)2;
		a = modularMultUniver(a, a, module);

	}
	return res;
}

ulong modularInverseAddUniver(ulong a, ulong module)
{
	return module - (a % module);
}

ulong modularInverseMultUniver(ulong a, ulong module)
{
	return modularDegUniver(a, euler(module) - 1, module);
}

/**
 *	Other
 */

//	Euler function
ulong euler(ulong n) {
	ulong result = n, i;
	for (i = 2; i * i <= n; ++i)
		if (n % i == 0) {
			while (n % i == 0)
				n /= i;
			result -= result / i;
		}
	if (n > 1)
		result -= result / n;
	return result;
}

//	gcd
ulong gcd(ulong a, ulong b) {
	ulong c;
	while (a != 0) {
		c = a; a = b % a;  b = c;
	}
	return b;
}
