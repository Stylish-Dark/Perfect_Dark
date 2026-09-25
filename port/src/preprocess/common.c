#include "preprocess/common.h"

static struct ptrmarker ptrMarkers[MAX_PTR_MARKERS];
static int numPtrMarkers;
static s32 preprocessFileNum = -1;

void preprocessSetFileNum(s32 fileNum)
{
	preprocessFileNum = fileNum;
}

s32 preprocessGetFileNum(void)
{
	return preprocessFileNum;
}

void ptrAdd(u32 ptr_src, uintptr_t ptr_host)
{
	if (numPtrMarkers >= ARRAYCOUNT(ptrMarkers)) {
		sysFatalError("ptrAdd(%p, %p): ptrmarker limit exceeded", (void *)(uintptr_t)ptr_src, (void *)ptr_host);
	}

	ptrMarkers[numPtrMarkers].ptr_src = ptr_src;
	ptrMarkers[numPtrMarkers].ptr_host = ptr_host;
	numPtrMarkers++;
}

struct ptrmarker *ptrFind(uintptr_t ptr_src)
{
	for (int i = 0; i < numPtrMarkers; i++) {
		if (ptrMarkers[i].ptr_src == ptr_src) {
			return &ptrMarkers[i];
		}
	}

	return NULL;
}

void ptrReset(void)
{
	numPtrMarkers = 0;
}
