#pragma once

#include "sequence_motor_info.h"


struct SequenceInfo
{
	TransportModuleInfo < Position, Speed, FullPosition > transport;

	SequenceInfo();

//	void _loadParameters(InspectionParameter & parameter);
//	void _saveParameters(InspectionParameter & parameter) const;
};

struct SequenceLimit
{
	TransportModuleInfo < EmptyPosition, SpeedLimit, PositionLimit > transport;

	SequenceLimit();

	/*void _loadParameters(InspectionParameter & parameter);
	void _saveParameters(InspectionParameter & parameter) const;*/
};

struct SequenceResolution
{
	TransportModuleResolution transport;
};

bool ValidateSequenceSpeed( SequenceInfo & info, const SequenceLimit & limit, const SequenceResolution & resolution );

struct SubstrateInfo
{
	int blockColumnCount;
	int blockRowCount;
	double blockPitchX;
	double blockPitchY;
	int dieColumnPerBlock;
	int dieRowPerBlock;
	double diePitchX;
	double diePitchY;

	SubstrateInfo();

	/*void _loadParameters(InspectionParameter & parameter);
	void _saveParameters(InspectionParameter & parameter) const;*/
};