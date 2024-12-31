// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/StringBuilder.h"
#include <type_traits>
#include "GCConcepts.h"
#include <concepts>

namespace GCUtils::String
{
    template <class T, class TCharType>
    concept StringBuilderAppenderFunctor = GCConcepts::CharType<TCharType> && std::invocable<T, TStringBuilderBase<TCharType>&>;

    /**
     * @brief Utility struct to offload implementation of the string builder append operator to
     *        a callback functor.
     * @tparam TCharType Type of character in the string builders to support.
     * @tparam TFunctor Type of the callback functor to store.
     */
    template
        <
        GCConcepts::CharType TCharType,
        StringBuilderAppenderFunctor<TCharType> TFunctor
        >
    struct TStringBuilderAppender
    {
    public:

        static_assert(std::is_reference_v<TFunctor> == false, "The type template argument of the functor must not be a reference.");
        static_assert(std::is_const_v<TFunctor> == false, "The type template argument of the functor must not be const.");
        static_assert(std::is_volatile_v<TFunctor> == false, "The type template argument of the functor must not be volatile.");

    public:

        typedef TCharType FCharType;
        typedef TFunctor FFunctor;

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
     * @param inCallbackFunctorRef A forwarding reference to the functor to be called on in the
     *                             string builder append operator overload.
     * @return A prvalue of the constructed `TStringBuilderAppender<>`.
     */
    template
        <
        GCConcepts::CharType TCharType,
        class TFunctorRef,
        // Notice that `TFunctorRef&&` is a forwarding reference. This entails `TFunctorRef` may have a reference
        // baked into it and possibly cv-qualifiers as well. We make sure to remove those when passing it in as
        // the `TFunctor` template argument of the `TStringBuilderAppender<>` class.
        StringBuilderAppenderFunctor<TCharType> TFunctor = std::remove_cv_t<std::remove_reference_t<TFunctorRef>>
        >
    TStringBuilderAppender<TCharType, TFunctor> ConstructStringBuilderAppender(
        TFunctorRef&& inCallbackFunctorRef);
}

template
    <
    GCConcepts::CharType TCharType,
    class TFunctorRef,
    GCUtils::String::StringBuilderAppenderFunctor<TCharType> TFunctor
    >
GCUtils::String::TStringBuilderAppender<TCharType, TFunctor> GCUtils::String::ConstructStringBuilderAppender(
    TFunctorRef&& inCallbackFunctorRef)
{
    return TStringBuilderAppender<TCharType, TFunctor>(Forward<TFunctorRef>(inCallbackFunctorRef));
}
