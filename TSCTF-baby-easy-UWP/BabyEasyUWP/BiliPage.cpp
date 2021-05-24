#include "pch.h"
#include "BiliPage.h"
#if __has_include("BiliPage.g.cpp")
#include "BiliPage.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::BabyEasyUWP::implementation
{
    BiliPage::BiliPage()
    {
        InitializeComponent();
    }

    int32_t BiliPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void BiliPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    //void BiliPage::HyperlinkButton_Click(
    //    winrt::Windows::Foundation::IInspectable const& sender,
    //    winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    //{
    //    this->Frame().Navigate(xaml_typename<BabyEasyUWP::MainPage>());
    //}
}
