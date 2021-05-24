#pragma once

#include "MainPage.g.h"

namespace winrt::BabyEasyUWP::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        const char* cipertext = "cdee\nme\x17gfaabx\u00164\x1d`\u0010f6\x10>794;bf&g,e\x0cm";
        MainPage();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void inputButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void HyperlinkButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::BabyEasyUWP::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
