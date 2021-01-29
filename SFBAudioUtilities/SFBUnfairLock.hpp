/*
 * Copyright (c) 2020 - 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

#import <os/lock.h>

/*!
 * @brief A wrapper around @c os_unfair_lock implementing C++ Lockable
 *
 * This class may be used with @c std::lock_guard for a scope-based lock.
 *
 * @code
 * SFBUnfairLock _lock;
 * // Later
 * std::lock_guard<SFBUnfairLock> lock(_lock);
 * @endcode
 *
 */
class SFBUnfairLock
{

public:

	// ========================================
	/*! @name Creation and Destruction */
	//@{

	/*! @brief Create a new @c SFBUnfairLock */
	inline SFBUnfairLock() noexcept
	: mLock(OS_UNFAIR_LOCK_INIT)
	{}

	/*! @internal This class is non-copyable */
	SFBUnfairLock(const SFBUnfairLock& rhs) = delete;

	/*! @internal This class is non-assignable */
	SFBUnfairLock& operator=(const SFBUnfairLock& rhs) = delete;

	//@}


	// ========================================
	/*! @name Lockable */
	//@{

	/*!@brief Lock the lock. */
	inline void lock() noexcept
	{
		os_unfair_lock_lock(&mLock);
	}

	/*!@brief Unlock the lock. */
	inline void unlock() noexcept
	{
		os_unfair_lock_unlock(&mLock);
	}

	/*!
	 * @brief Attempt to lock the lock.
	 * @return @c true if the lock was successfully locked, @c false on error
	 */
	inline bool try_lock() noexcept
	{
		return os_unfair_lock_trylock(&mLock);
	}

	//@}

private:

	os_unfair_lock		mLock;					/*!< The primitive lock */

};
