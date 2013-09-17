#include "mex.h"

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;

template<class T>
void BoxFilter(const T *I, int ih, int iw, int bh, int bw, T *J)
{
	int iih = ih + 2 * bh + 1, iiw = iw + 2 * bw + 1;
	T *II = (T *) mxMalloc(iih * iiw * sizeof(T));

	{
		const T *ip = I;
		T *iip = II;
		for (int i = 0; i < iih; i++)
			*(iip++) = 0;
		for (int x = -bw; x < 0; x++)
		{
			*(iip++) = 0;
			for (int i = 0; i < bh; i++)
				*(iip++) = ip[0];
			for (int i = 0; i < ih; i++)
				*(iip++) = ip[i];
			for (int i = 0; i < bh; i++)
				*(iip++) = ip[ih - 1];
		}
		for (int x = 0; x < iw; x++)
		{
			*(iip++) = 0;
			for (int i = 0; i < bh; i++)
				*(iip++) = ip[0];
			for (int i = 0; i < ih; i++)
				*(iip++) = *(ip++);
			for (int i = 0; i < bh; i++)
				*(iip++) = ip[-1];
		}
		ip -= ih;
		for (int x = 0; x < bw; x++)
		{
			*(iip++) = 0;
			for (int i = 0; i < bh; i++)
				*(iip++) = ip[0];
			for (int i = 0; i < ih; i++)
				*(iip++) = ip[i];
			for (int i = 0; i < bh; i++)
				*(iip++) = ip[ih - 1];
		}
	}

	{
		T *iip = II + iih + 1;
		for (int x = 0; x < iiw - 1; x++, iip++)
			for (int y = 0; y < iih - 1; y++, iip++)
				*iip = iip[-1] + iip[-iih] + *iip - iip[-iih-1];
	}

	{
		int	o1 = 2 * bh + 1, o2 = (2 * bw + 1) * iih, o3 = o1 + o2;
		int iioff = iih - ih;

		const T *ip = II;
		T *jp = J;
		for (int x = 0; x < iw; x++, ip += iioff)
			for (int y = 0; y < ih; y++, ip++, jp++)
				*jp = *ip - ip[o1] - ip[o2] + ip[o3];
	}
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (nrhs != 3)
		mexErrMsgTxt("three input arguments expected");

	if (nlhs != 1)
		mexErrMsgTxt("one output argument expected");
	
	if (mxIsComplex(prhs[0]) ||
		!(mxIsSingle(prhs[0]) || mxIsDouble(prhs[0]) ||
            mxIsUint8(prhs[0]) || mxIsUint16(prhs[0]) || mxIsUint32(prhs[0])) ||
		mxGetNumberOfDimensions(prhs[0]) != 2)
		mexErrMsgTxt("argument 1 (I) must be a single/double/uint8/uint16/uint32 matrix");

	int bh;
	if (mxIsComplex(prhs[1]) || !mxIsDouble(prhs[1]) ||
		(bh = (int) mxGetScalar(prhs[1])) != mxGetScalar(prhs[1]) || bh < 0)
		mexErrMsgTxt("argument 2 (bh) must be a positive integer-valued scalar");

	int bw;
	if (mxIsComplex(prhs[2]) || !mxIsDouble(prhs[2]) ||
		(bw = (int) mxGetScalar(prhs[2])) != mxGetScalar(prhs[2]) || bh < 0)
		mexErrMsgTxt("argument 2 (bw) must be a positive integer-valued scalar");

	int ih = mxGetM(prhs[0]), iw = mxGetN(prhs[0]);

	switch (mxGetClassID(prhs[0]))
	{
		case mxSINGLE_CLASS:
			{
				plhs[0] = mxCreateNumericMatrix(ih, iw, mxSINGLE_CLASS, mxREAL);
				float *J = (float *) mxGetData(plhs[0]);
				const float *I = (const float *) mxGetData(prhs[0]);
				BoxFilter(I, ih, iw, bh, bw, J);
			}
			break;

		case mxDOUBLE_CLASS:
			{
				plhs[0] = mxCreateNumericMatrix(ih , iw, mxDOUBLE_CLASS, mxREAL);
				double *J = (double *) mxGetData(plhs[0]);
				const double *I = (const double *) mxGetData(prhs[0]);
				BoxFilter(I, ih, iw, bh, bw, J);
			}
			break;

		case mxUINT8_CLASS:
			{
				plhs[0] = mxCreateNumericMatrix(ih , iw, mxUINT8_CLASS, mxREAL);
				uint8_t *J = (uint8_t *) mxGetData(plhs[0]);
				const uint8_t *I = (const uint8_t *) mxGetData(prhs[0]);
				BoxFilter(I, ih, iw, bh, bw, J);
			}
			break;

		case mxUINT16_CLASS:
			{
				plhs[0] = mxCreateNumericMatrix(ih , iw, mxUINT16_CLASS, mxREAL);
				uint16_t *J = (uint16_t *) mxGetData(plhs[0]);
				const uint16_t *I = (const uint16_t *) mxGetData(prhs[0]);
				BoxFilter(I, ih, iw, bh, bw, J);
			}
			break;

		case mxUINT32_CLASS:
			{
				plhs[0] = mxCreateNumericMatrix(ih , iw, mxUINT32_CLASS, mxREAL);
				uint32_t *J = (uint32_t *) mxGetData(plhs[0]);
				const uint32_t *I = (const uint32_t *) mxGetData(prhs[0]);
				BoxFilter(I, ih, iw, bh, bw, J);
			}
			break;
	}	
}
