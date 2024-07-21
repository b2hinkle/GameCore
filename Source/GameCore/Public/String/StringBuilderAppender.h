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

        static_assert(TIsReferenceType<TFunctor>::Value == false, "The type template argument of the functor must not be a reference.");

    public:

        /**
         * @brief Construct from a callback functor.
         * @param inCallbackFunctor Functor to be called on in the string builder append
         *                          operator overload.
         */
        TStringBuilderAppender(const TFunctor& inCallbackFunctor)
            : CallbackFunctor(inCallbackFunctor)
        {
        }

        /**
         * @brief Construct from a callback functor.
         * @param inCallbackFunctor Functor to be called on in the string builder append
         *                          operator overload.
         */
        TStringBuilderAppender(TFunctor&& inCallbackFunctor)
            : CallbackFunctor(MoveTemp(inCallbackFunctor))
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
     * @brief Wrapper function of the `TStringBuilderAppender<>` constructors to provide function
     *        template deduction rules when constructing the template class.
     * @tparam TCharType Type of character in the string builders to support.
     * @tparam TFunctorRef Reference to the type of the callback functor to store.
     * @param inCallbackFunctor Forwarding reference to the functor to be called on in the string
     *                          builder append operator overload.
     * @return A prvalue of the constructed `TStringBuilderAppender<>`.
     */
    template <class TCharType, class TFunctorRef>
    TStringBuilderAppender<TCharType, typename TRemoveReference<TFunctorRef>::Type>
        ConstructStringBuilderAppender(TFunctorRef&& inCallbackFunctor);
}

template <class TCharType, class TFunctorRef>
GCUtils::String::TStringBuilderAppender<TCharType, typename TRemoveReference<TFunctorRef>::Type>
    GCUtils::String::ConstructStringBuilderAppender(TFunctorRef&& inCallbackFunctor)
{
    // Note: `TFunctorRef` has a reference baked into it since `TFunctorRef&&` is a forwarding reference. We
    // make sure to remove that when passing it in as the `TFunctor` argument.
    return TStringBuilderAppender<TCharType, typename TRemoveReference<TFunctorRef>::Type>(
        Forward<TFunctorRef>(inCallbackFunctor));
}
