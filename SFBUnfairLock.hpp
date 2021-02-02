//
// Copyright (c) 2020 - 2021 Stephen F. Booth <me@sbooth.org>
// MIT license
//

#pragma once

#import <os/lock.h>

/// A wrapper around @c os_unfair_lock implementing C++ @c Lockable
///
/// This class may be used with @c std::lock_guard for a scope-based lock.
///
/// @code
/// SFBUnfairLock _lock;
/// // Later
/// std::lock_guard<SFBUnfairLock> lock(_lock);
/// @endcode
class SFBUnfairLock
{

public:

#pragma mark Creation and Destruction

	/// Creates a new @c SFBUnfairLock
	inline SFBUnfairLock() noexcept
	: mLock(OS_UNFAIR_LOCK_INIT)
	{}

	// This class is non-copyable
	SFBUnfairLock(const SFBUnfairLock& rhs) = delete;

	// This class is non-assignable
	SFBUnfairLock& operator=(const SFBUnfairLock& rhs) = delete;

	// Destructor
	~SFBUnfairLock() = default;

	// This class is non-movable
	SFBUnfairLock(const SFBUnfairLock&& rhs) = delete;

	// This class is non-move assignable
	SFBUnfairLock& operator=(const SFBUnfairLock&& rhs) = delete;

#pragma mark Lockable

	/// Locks the lock
	inline void lock() noexcept
	{
		os_unfair_lock_lock(&mLock);
	}

	/// Unlocks the lock
	inline void unlock() noexcept
	{
		os_unfair_lock_unlock(&mLock);
	}

	/// Attempts to lock the lock
	/// @return @c true if the lock was successfully locked, @c false on error
	inline bool try_lock() noexcept
	{
		return os_unfair_lock_trylock(&mLock);
	}

private:

	/// The primitive lock
	os_unfair_lock mLock;

};
