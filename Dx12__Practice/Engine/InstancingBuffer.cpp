#include "pch.h"
#include "InstancingBuffer.h"

InstancingBuffer::InstancingBuffer()
{
}

InstancingBuffer::~InstancingBuffer()
{
}

void InstancingBuffer::ClearData()
{
	_data.clear();
}

void InstancingBuffer::AddData(InstancingData& data)
{
	_data.push_back(data);
}

void InstancingBuffer::PushData()
{
	const int32 dataCount = GetCount();
	if (dataCount > _maxCount)
		CreateBuffer(dataCount);
}

void InstancingBuffer::CreateBuffer(uint32 maxCount)
{
	_maxCount = maxCount;


	//_instanceBuffer = make_shared<VertexBuffer>();

	vector<InstancingData> temp(maxCount);
}
