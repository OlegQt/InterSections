#pragma once

enum LineTypes
{
	FORSE,
	VELOCITY,
	BOND,
	PROECTION
};

struct rvector {
	float x, y;
};
struct rLine {
	rvector a, b;
	int type;
};

struct element {
	rvector pos;
	rvector V;
	float mass,diameter;
};