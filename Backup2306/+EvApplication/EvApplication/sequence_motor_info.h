#pragma once
#include <string>

//#include "inspectionParameter.h"
#include "position_info.h"

struct MotorInfo
{
	MotorInfo() { _motorName = "Unknown motor"; };

	void _setMotorName(std::string name) { _motorName = name; };

protected:
	std::string _motorName;
};

template <typename positionT, typename speedT, typename fullPositionT>
struct XMotor : MotorInfo
{
	fullPositionT ready;
	speedT startScanning;
	speedT scanning;
	fullPositionT afterScanning;
	speedT returnReady;

	XMotor()
	{
		_setMotorName( "X" );

		ready._set( "Ready", -29491, 100, 200000 );
		startScanning._set( "Start Scanning", 100, 200 );
		scanning._set( "Scanning", 100, 200 );
//		bondStartScanning._set( "Bond Start Scanning", 0, 100, 400000 );
		afterScanning._set( "After Scanning", 469102, 100, 204800 );
		returnReady._set( "Return Ready", 100, 409600 );
	};

	/*void _loadParameters(InspectionParameter & parameter, const std::string & sectionName)
	{
		ready._loadParameters( parameter, sectionName, _motorName );
		startScanning._loadParameters( parameter, sectionName, _motorName );
		scanning._loadParameters( parameter, sectionName, _motorName );
		bondStartScanning._loadParameters( parameter, sectionName, _motorName );
		afterScanning._loadParameters( parameter, sectionName, _motorName );
		returnReady._loadParameters( parameter, sectionName, _motorName );
	};*/

	/*void _saveParameters(InspectionParameter & parameter, const std::string & sectionName) const
	{
		ready._saveParameters( parameter, sectionName, _motorName );
		startScanning._saveParameters( parameter, sectionName, _motorName );
		scanning._saveParameters( parameter, sectionName, _motorName );
		bondStartScanning._saveParameters( parameter, sectionName, _motorName );
		afterScanning._saveParameters( parameter, sectionName, _motorName );
		returnReady._saveParameters( parameter, sectionName, _motorName );
	};*/
};

template <typename positionT, typename speedT, typename fullPositionT>
struct YMotor : MotorInfo
{
	fullPositionT ready;
	speedT startScanning;
	speedT scanning;
	fullPositionT afterScanning;

	YMotor()
	{
		_setMotorName( "Y" );

		ready._set( "Ready", 1638, 100, 400000 );
		startScanning._set( "Start Scanning", 100, 200 );
		scanning._set( "Scanning", 100, 200 );
		afterScanning._set( "After Scanning", 1638, 100, 410000 );
	};

	/*void _loadParameters(InspectionParameter & parameter, const std::string & sectionName)
	{
		ready._loadParameters( parameter, sectionName, _motorName );
		startScanning._loadParameters( parameter, sectionName, _motorName );
		scanning._loadParameters( parameter, sectionName, _motorName );
		bondStartScanning._loadParameters( parameter, sectionName, _motorName );
		afterScanning._loadParameters( parameter, sectionName, _motorName );
	};

	void _saveParameters(InspectionParameter & parameter, const std::string & sectionName) const
	{
		ready._saveParameters( parameter, sectionName, _motorName );
		startScanning._saveParameters( parameter, sectionName, _motorName );
		scanning._saveParameters( parameter, sectionName, _motorName );
		bondStartScanning._saveParameters( parameter, sectionName, _motorName );
		afterScanning._saveParameters( parameter, sectionName, _motorName );
	};*/
};


template <typename positionT, typename speedT, typename fullPositionT>
struct ZMotor : MotorInfo
{
	fullPositionT ready;
	speedT startScanning;
	speedT scanning;
	fullPositionT afterScanning;

	ZMotor()
	{
		_setMotorName( "Z" );

		ready._set("Ready", 1638, 100, 400000);
		startScanning._set("Start Scanning", 100, 200);
		scanning._set("Scanning", 100, 200);
		afterScanning._set("After Scanning", 1638, 100, 410000);
	};

	/*void _loadParameters(InspectionParameter & parameter, const std::string & sectionName)
	{
		startScanning._loadParameters( parameter, sectionName, _motorName );
	};

	void _saveParameters(InspectionParameter & parameter, const std::string & sectionName) const
	{
		startScanning._saveParameters( parameter, sectionName, _motorName );
	};*/
};



template <typename positionT, typename speedT, typename fullPositionT>
struct TransportModuleInfo
{
	XMotor < positionT, speedT, fullPositionT > x;
	YMotor < positionT, speedT, fullPositionT > y;
	ZMotor < positionT, speedT, fullPositionT > z;

	TransportModuleInfo() : _sectionName( "Unknown module" ) {};

	/*void _loadParameters(InspectionParameter & parameter)
	{
		width._loadParameters( parameter, _sectionName );
		x._loadParameters( parameter, _sectionName );
		y._loadParameters( parameter, _sectionName );
		pushPull._loadParameters( parameter, _sectionName );
		bondZ._loadParameters( parameter, _sectionName );
		Z_23DFocusing._loadParameters( parameter, _sectionName );
	};

	void _saveParameters(InspectionParameter & parameter) const
	{
		width._saveParameters( parameter, _sectionName );
		x._saveParameters( parameter, _sectionName );
		y._saveParameters( parameter, _sectionName );
		pushPull._saveParameters( parameter, _sectionName );
		bondZ._saveParameters( parameter, _sectionName );
		Z_23DFocusing._saveParameters( parameter, _sectionName );
	};*/

	void _setSectionName(std::string name) { _sectionName = name; };

private:
	std::string _sectionName;
};

struct TransportModuleResolution
{
	double x;
	double y;
	double z;

	TransportModuleResolution() : x( 1 ), y( 1 ), z( 1 ) { };
};