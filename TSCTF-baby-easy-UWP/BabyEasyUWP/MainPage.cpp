#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include "BiliPage.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::BabyEasyUWP::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();
    }

    int32_t MainPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
    char* resultBuffer;
    int counter = 0;
    void step1(char* buffer, int len) {
        for (int i = 0; i < len; i++) {
            buffer[i] ^= 'U';
        }
    }
    void step2(char* buffer, char* resultBuffer, int root, int len) {
        if (root > len) {
            return;
        }
        step2(buffer, resultBuffer, 2 * root, len);
        resultBuffer[counter++] = buffer[root - 1];
        step2(buffer, resultBuffer, 2 * root + 1, len);
    }

    void MainPage::inputButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        counter = 0;
        auto flag_input = flagInput().Text();
        std::string flag = winrt::to_string(flag_input);
        size_t len = flag.size();
        char* buffer = new char[len+1];
        char* resultBuffer = new char[len + 1];
        strcpy_s(buffer, len+1, flag.c_str());
        step2(buffer, resultBuffer, 1, len);
        resultBuffer[len] = '\x00';
        step1(resultBuffer, len);
        if (!strcmp(resultBuffer, this->cipertext)) {
            checkOutput().Text(L"Right!!!");
        }
        else {
            checkOutput().Text(L"Wrong flag!");
        }
        delete[] buffer;
        delete[] resultBuffer;
        return;
    }
    
    void MainPage::HyperlinkButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        this->Frame().Navigate(xaml_typename<BabyEasyUWP::BiliPage>());
    }
}
