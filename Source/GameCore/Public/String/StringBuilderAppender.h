// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"

namespace GCUtils::String
{
    /**
     * @brief Utility struct to offload implementation of the string builder append operator to
     *        a callback functor.
     * @tparam TFunctor Type of the callback functor to store.
     */
    template
        <
        class TFunctor,
        class TCharType = TCHAR,
        class = typename TEnableIf
            <
            TIsInvocable<TFunctor, TStringBuilderBase<TCharType>&>::Value
            >::Type
        >
    struct TStringBuilderAppender
    {
    public:

        /**
         * @brief Construct from a callback functor.
         * @param inCallbackFunctor Functor to be called on in the string builder append
         *                          operator overload.
         */
        TStringBuilderAppender(TFunctor&& inCallbackFunctor)
            : CallbackFunctor(Forward<TFunctor>(inCallbackFunctor))
        {
        }

    public:

        friend TStringBuilderBase<TCharType>& operator<<(TStringBuilderBase<TCharType>& inStringBuilder,
            const TStringBuilderAppender& inStringBuilderAppender)
        {
            return inStringBuilderAppender.CallbackFunctor(inStringBuilder);
        }

    protected:

        TFunctor CallbackFunctor;
    };
}
