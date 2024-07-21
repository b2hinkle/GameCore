// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"

namespace GCUtils::String
{
    /**
     * @brief Utility struct to offload implementation of the string builder append operator to
     *        a callback functor.
     * @tparam TCharType Type of character in the string builders to support.
     * @tparam TFunctor Type of the callback functor to store.
     */
    template
        <
        class TCharType,
        class TFunctor,
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

        friend TStringBuilderBase<TCharType>& operator<<(TStringBuilderBase<TCharType>& inStringBuilder,
            TStringBuilderAppender&& inStringBuilderAppender)
        {
            return MoveTemp(inStringBuilderAppender.CallbackFunctor)(inStringBuilder);
        }

    protected:

        TFunctor CallbackFunctor;
    };

    /**
     * @brief Wrapper function of the `TStringBuilderAppender<>` constructor to provide function
     *        template deduction rules when constructing the template class.
     * @tparam TCharType Type of character in the string builders to support.
     * @tparam TFunctor Type of the callback functor to store.
     * @param inCallbackFunctor Functor to be called on in the string builder append
     *                          operator overload.
     * @return A prvalue of the constructed `TStringBuilderAppender<>`.
     */
    template <class TCharType, class TFunctor>
    TStringBuilderAppender<TCharType, TFunctor> ConstructStringBuilderAppender(TFunctor&& inCallbackFunctor)
    {
        return TStringBuilderAppender<TCharType, TFunctor>(Forward<TFunctor>(inCallbackFunctor));
    }
}
