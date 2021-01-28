/*
 * Copyright (c) 2021 Stephen F. Booth <me@sbooth.org>
 * MIT license
 */

#pragma once

/*!
 * @brief A class that calls a closure upon destruction
 *
 * This allows similar functionality to \c defer  in Swift and Go.
 * This is useful to limit the lifetime of non-C++ objects and provides an alternative to \c std::unique_ptr  with a
 * custom deleter.
 *
 * \code
 * ExtAudioFileRef eaf;
 * auto result = ExtAudioFileOpenURL(url, &eaf);
 * assert(result == noErr);
 * auto lambda = [eaf]() {
 *     auto result = ExtAudioFileDispose(eaf);
 *     assert(result == noErr);
 * };
 * SFBDeferredClosure<decltype(lambda)> cleanup(lambda);
 * \endcode
 *
 */
template <typename F>
class SFBDeferredClosure
{

public:

	/*! Creates a new \c SFBDeferredClosure executing \c closure when the destructor is called */
	inline SFBDeferredClosure(const F& closure)
	: mClosure(closure)
	{}

	/*! @internal This class is non-copyable */
	SFBDeferredClosure(const SFBDeferredClosure& rhs) = delete;

	/*! @internal This class is non-assignable */
	SFBDeferredClosure& operator=(const SFBDeferredClosure& rhs) = delete;

	/*! Executes the closure */
	inline ~SFBDeferredClosure()
	{
		mClosure();
	}

private:

	const F& mClosure;

};
