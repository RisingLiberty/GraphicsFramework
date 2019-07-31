#pragma once

class Dx12Command
{
public:
	Dx12Command();
	virtual ~Dx12Command();

	virtual void Execute() = 0;

};