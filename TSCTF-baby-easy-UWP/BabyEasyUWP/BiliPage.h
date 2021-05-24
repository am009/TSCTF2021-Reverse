#pragma once

#include "BiliPage.g.h"

namespace winrt::BabyEasyUWP::implementation
{
    struct BiliPage : BiliPageT<BiliPage>
    {
        BiliPage();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void HyperlinkButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::BabyEasyUWP::factory_implementation
{
    struct BiliPage : BiliPageT<BiliPage, implementation::BiliPage>
    {
    };
}
