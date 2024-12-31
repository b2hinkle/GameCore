// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * @brief Container utils.
 */
namespace GCUtils::Container
{
    /**
     * @brief Make an array view that interprets the elements as another type. Done
     *        by const-casting the data pointer.
     */
    template
        <
        typename TToElementType,
        typename TFromElementType,
        typename TSizeType
        >
    TArrayView<TToElementType, TSizeType> ConstCastArrayViewDataPtr(
        const TArrayView<TFromElementType, TSizeType>& inFromArray);

    /**
     * @brief Make an array view that interprets the elements as another type. Done
     *        by static-casting the data pointer.
     */
    template
        <
        typename TToElementType,
        typename TFromElementType,
        typename TSizeType
        >
    TArrayView<TToElementType, TSizeType> StaticCastArrayViewDataPtr(
        const TArrayView<TFromElementType, TSizeType>& inFromArray);

    /**
     * @brief Make an array view that interprets the elements as another type. Done
     *        by reinterpret-casting the data pointer.
     */
    template
        <
        typename TToElementType,
        typename TFromElementType,
        typename TSizeType
        >
    TArrayView<TToElementType, TSizeType> ReinterpretCastArrayViewDataPtr(
        const TArrayView<TFromElementType, TSizeType>& inFromArray);
}

template
    <
    typename TToElementType,
    typename TFromElementType,
    typename TSizeType
    >
TArrayView<TToElementType, TSizeType> GCUtils::Container::ConstCastArrayViewDataPtr(
    const TArrayView<TFromElementType, TSizeType>& inFromArray)
{
    static_assert(sizeof(TToElementType) == sizeof(TFromElementType), "Element sizes should be same! Or else element access would be scaled!");

    return TArrayView<TToElementType, TSizeType>(
        const_cast<TToElementType*>(inFromArray.GetData()),
        inFromArray.Num()
        );
}

template
    <
    typename TToElementType,
    typename TFromElementType,
    typename TSizeType
    >
TArrayView<TToElementType, TSizeType> GCUtils::Container::StaticCastArrayViewDataPtr(
    const TArrayView<TFromElementType, TSizeType>& inFromArray)
{
    static_assert(sizeof(TToElementType) == sizeof(TFromElementType), "Element sizes should be same! Or else element access would be scaled!");

    return TArrayView<TToElementType, TSizeType>(
        static_cast<TToElementType*>(inFromArray.GetData()),
        inFromArray.Num()
        );
}

template
    <
    typename TToElementType,
    typename TFromElementType,
    typename TSizeType
    >
TArrayView<TToElementType, TSizeType> GCUtils::Container::ReinterpretCastArrayViewDataPtr(
    const TArrayView<TFromElementType, TSizeType>& inFromArray)
{
    static_assert(sizeof(TToElementType) == sizeof(TFromElementType), "Element sizes should be same! Or else element access would be scaled!");

    return TArrayView<TToElementType, TSizeType>(
        reinterpret_cast<TToElementType*>(inFromArray.GetData()),
        inFromArray.Num()
        );
}
