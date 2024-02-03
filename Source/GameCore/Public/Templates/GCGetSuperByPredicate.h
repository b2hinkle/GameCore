// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Recursively traverse the Super chain until the TPredicate is met
 */
template
<
	class TCurrentType,
	template <class> class TPredicate,
	bool bCurrentTypePassesPredicate = false
>
struct TGCGetSuperByPredicate;

/**
 * Case for TCurrentType DOES NOT pass the TPredicate
 */
template
<
	class TCurrentType,
	template <class> class TPredicate
>
struct TGCGetSuperByPredicate<TCurrentType, TPredicate, false>
{
protected:
	enum
	{
		// Does the TCurrentType's Super pass the TPredicate?
		bSuperPassesPredicate = TPredicate<typename TCurrentType::Super>::Value
	};

	// Recursively test the Supers
	using RecursiveResult = typename TGCGetSuperByPredicate<typename TCurrentType::Super, typename TPredicate, bSuperPassesPredicate>::Result;

public:
	// Use the next recursive test as the Result
	typedef RecursiveResult Result;
};

/**
 * Case for TCurrentType DOES pass the TPredicate
 */
template
<
	class TCurrentType,
	template <class> class TPredicate
>
struct TGCGetSuperByPredicate<TCurrentType, TPredicate, true>
{
public:
	// We pass the predicate!
	// So stop recursing and use us for the Result
	typedef typename TCurrentType Result;
};
