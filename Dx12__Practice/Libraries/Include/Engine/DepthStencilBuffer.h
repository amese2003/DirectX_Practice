#pragma once


class DepthStencilBuffer
{
public:
	DepthStencilBuffer();
	~DepthStencilBuffer();

	void Init();

private:
	shared_ptr<Texture> _texture;
};

