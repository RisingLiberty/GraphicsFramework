#include "stdafx.h"

#include "Vertex.h"

bool Vertex::operator==(const Vertex& other) const
{
	return Position == other.Position && Color == other.Color && TexCoord == other.TexCoord;
}