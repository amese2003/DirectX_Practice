#pragma once


class TimeManager
{
	DECLARE_SINGLE(TimeManager);

public:
	void Init();
	void Update();


	float TotalTime() const;  // in seconds
	float DeltaTime() const; // in seconds

	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick();  // Call every frame.

private:
	double _secondsPerCount = 0.0;
	double _deltaTime = -1.0;
	int64 _baseTime = 0;
	int64 _pausedTime = 0;
	int64 _stopTime = 0;
	int64 _prevTime = 0;
	int64 _currTime = 0;
	bool _stopped = false;


public:
	uint32 GetFps() { return _fps; }
	//void CalculateFrameStats();
	//float GetDeltaTime() { return _deltaTime; }

private:
	uint64	_frequency = 0;
	uint64	_prevCount = 0;

private:
	uint32	_frameCount = 0;
	float	_frameTime = 0.f;
	uint32	_fps = 0;
};

