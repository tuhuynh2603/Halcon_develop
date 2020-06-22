#include "stdafx.h"
#include "sequence_info.h"
#include "operatorOverloading.h"

//template <class T>
//void get_value( InspectionParameter & parameter, std::string sectionName, std::string itemName, T & value )
//{
//	value = parameter.get( sectionName, itemName, value );
//};



//void MagazineInfo::_loadParameters(InspectionParameter & parameter)
//{
//	get_value( parameter, _sectionName, "Height (mm)", heightMm );
//	get_value( parameter, _sectionName, "Length (mm)", lengthMm );
//	get_value( parameter, _sectionName, "Width  (mm)", widthMm );
//	get_value( parameter, _sectionName, "First Position Offset (mm)", firstPositionOffsetMm );
//	get_value( parameter, _sectionName, "Pitch (mm)", pitchMm );
//	get_value( parameter, _sectionName, "Loader Manual Position (pps)", loaderManualPosition );
//	get_value( parameter, _sectionName, "Unloader Manual Position (pps)", unloaderManualPosition );
//	get_value( parameter, _sectionName, "Transport Focus Manual Position (pps)", transportFocusManualPosition );
//	get_value( parameter, _sectionName, "Number of substrates", substrateCount );
//	get_value( parameter, _sectionName, "Substrates width (mm)", substrateWidthMm );
//	get_value( parameter, _sectionName, "Offset from bottom (mm)", bottomOffsetMm );
//}

//void MagazineInfo::_saveParameters(InspectionParameter & parameter) const
//{
//	parameter.set( _sectionName, "Height (mm)", heightMm );
//	parameter.set( _sectionName, "Length (mm)", lengthMm );
//	parameter.set( _sectionName, "Width  (mm)", widthMm );
//	parameter.set( _sectionName, "First Position Offset (mm)", firstPositionOffsetMm );
//	parameter.set( _sectionName, "Pitch (mm)", pitchMm );
//	parameter.set( _sectionName, "Loader Manual Position (pps)", loaderManualPosition );
//	parameter.set( _sectionName, "Unloader Manual Position (pps)", unloaderManualPosition );
//	parameter.set( _sectionName, "Transport Focus Manual Position (pps)", transportFocusManualPosition );
//	parameter.set( _sectionName, "Number of substrates", substrateCount );
//	parameter.set( _sectionName, "Substrates width (mm)", substrateWidthMm );
//	parameter.set( _sectionName, "Offset from bottom (mm)", bottomOffsetMm );
//}

//void MagazineInfo::_setSectionName(std::string name)
//{
//	_sectionName = name;
//}


SequenceInfo::SequenceInfo()
{
	transport._setSectionName( "Transport module" );
}

//void SequenceInfo::_loadParameters(InspectionParameter & parameter)
//{
//	loader._loadParameters( parameter );
//	transport._loadParameters( parameter );
//	unloader._loadParameters( parameter );
//	magazine._loadParameters( parameter );
//}

//void SequenceInfo::_saveParameters(InspectionParameter & parameter) const
//{
//	loader._saveParameters( parameter );
//	transport._saveParameters( parameter );
//	unloader._saveParameters( parameter );
//	magazine._saveParameters( parameter );
//}

SequenceLimit::SequenceLimit()
{
	transport._setSectionName( "Transport module" );
}

//void SequenceLimit::_loadParameters(InspectionParameter & parameter)
//{
//	loader._loadParameters( parameter );
//	transport._loadParameters( parameter );
//	unloader._loadParameters( parameter );
//}
//
//void SequenceLimit::_saveParameters(InspectionParameter & parameter) const
//{
//	loader._saveParameters( parameter );
//	transport._saveParameters( parameter );
//	unloader._saveParameters( parameter );
//}

bool ValidateData( Position &, const EmptyPosition &, double )
{
	return false;
}

bool ValidateData( Speed & speed, const SpeedLimit & limit, double resolution )
{
	int maximumLimit = static_cast<int>( limit.limit * resolution );

	bool limitSet = false;

	if( speed.maximumSpeed > maximumLimit ) {
		speed.maximumSpeed = maximumLimit;
		limitSet = true;
	}

	if( speed.startSpeed > maximumLimit ) {
		speed.startSpeed = maximumLimit;
		limitSet = true;
	}

	return limitSet;
}

bool ValidateData( FullPosition & speed, const PositionLimit & limit, double resolution )
{
	int maximumLimit = static_cast<int>( limit.limit * resolution );

	bool limitSet = false;

	if( speed.maximumSpeed > maximumLimit ) {
		speed.maximumSpeed = maximumLimit;
		limitSet = true;
	}

	if( speed.startSpeed > maximumLimit ) {
		speed.startSpeed = maximumLimit;
		limitSet = true;
	}

	return limitSet;
}

bool ValidateSequenceSpeed( SequenceInfo & info, const SequenceLimit & limit, const SequenceResolution & resolution )
{
	bool limitNotSet = true;
	
	// Transport Module
	limitNotSet = limitNotSet & !ValidateData( info.transport.x.ready							, limit.transport.x.ready							, resolution.transport.x );
	limitNotSet = limitNotSet & !ValidateData( info.transport.x.startScanning					, limit.transport.x.startScanning					, resolution.transport.x );
	limitNotSet = limitNotSet & !ValidateData( info.transport.x.scanning						, limit.transport.x.scanning						, resolution.transport.x );	
	limitNotSet = limitNotSet & !ValidateData( info.transport.x.afterScanning					, limit.transport.x.afterScanning					, resolution.transport.x );
	limitNotSet = limitNotSet & !ValidateData( info.transport.x.returnReady						, limit.transport.x.returnReady						, resolution.transport.x );
	limitNotSet = limitNotSet & !ValidateData( info.transport.y.ready							, limit.transport.y.ready							, resolution.transport.y );
	limitNotSet = limitNotSet & !ValidateData( info.transport.y.startScanning					, limit.transport.y.startScanning					, resolution.transport.y );
	limitNotSet = limitNotSet & !ValidateData( info.transport.y.scanning						, limit.transport.y.scanning						, resolution.transport.y );	
	limitNotSet = limitNotSet & !ValidateData( info.transport.y.afterScanning					, limit.transport.y.afterScanning					, resolution.transport.y );
	limitNotSet = limitNotSet & !ValidateData(info.transport.z.ready							, limit.transport.z.ready							, resolution.transport.z);
	limitNotSet = limitNotSet & !ValidateData(info.transport.z.startScanning					, limit.transport.z.startScanning					, resolution.transport.z);
	limitNotSet = limitNotSet & !ValidateData(info.transport.z.scanning							, limit.transport.z.scanning						, resolution.transport.z);	
	limitNotSet = limitNotSet & !ValidateData(info.transport.z.afterScanning					, limit.transport.z.afterScanning					, resolution.transport.z);

	
	return !limitNotSet;
}

SubstrateInfo::SubstrateInfo()
{
	blockColumnCount = 1;
	blockRowCount = 1;
	blockPitchX = 1;
	blockPitchY = 1;
	dieColumnPerBlock = 1;
	dieRowPerBlock = 1;
	diePitchX = 1;
	diePitchY = 1;
};

//void SubstrateInfo::_loadParameters(InspectionParameter & parameter)
//{
//	std::string sectionName = "Substrate information";
//
//	get_value( parameter, sectionName, "Block columns", blockColumnCount );
//	get_value( parameter, sectionName, "Block rows", blockRowCount );
//	get_value( parameter, sectionName, "Block pitch by X axis (mm)", blockPitchX );
//	get_value( parameter, sectionName, "Block pitch by Y axis (mm)", blockPitchY );
//	get_value( parameter, sectionName, "Die columns per block", dieColumnPerBlock );
//	get_value( parameter, sectionName, "Die rows per block", dieRowPerBlock );
//	get_value( parameter, sectionName, "Die pitch by X axis (mm)", diePitchX );
//	get_value( parameter, sectionName, "Die pitch by Y axis (mm)", diePitchY );
//}

//void SubstrateInfo::_saveParameters(InspectionParameter & parameter) const
//{
//	std::string sectionName = "Substrate information";
//
//	parameter.set( sectionName, "Block columns", blockColumnCount );
//	parameter.set( sectionName, "Block rows", blockRowCount );
//	parameter.set( sectionName, "Block pitch by X axis (mm)", blockPitchX );
//	parameter.set( sectionName, "Block pitch by Y axis (mm)", blockPitchY );
//	parameter.set( sectionName, "Die columns per block", dieColumnPerBlock );
//	parameter.set( sectionName, "Die rows per block", dieRowPerBlock );
//	parameter.set( sectionName, "Die pitch by X axis (mm)", diePitchX );
//	parameter.set( sectionName, "Die pitch by Y axis (mm)", diePitchY );
//}
