//
// MenuPage.xaml.cpp
// Implementation of the MenuPage class
//

#include "pch.h"
#include "MenuPage.xaml.h"
#include "StreamPage.xaml.h"
#include "Client\StreamConfiguration.h"
#include <string>
#include <ppltasks.h>
#include <Utils.hpp>

using namespace moonlight_xbox_dx;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Core;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

StreamConfiguration^ config = ref new StreamConfiguration();
MoonlightClient* client;

MenuPage::MenuPage() {
	//ref new StreamConfiguration();

	InitializeComponent();
	concurrency::create_task([] {
		config->initalizeUserFields();
		});
}

void moonlight_xbox_dx::MenuPage::ConnectButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	//config = ref new StreamConfiguration();
	Platform::String^ currentHostname = config->getHostname();

	if (currentHostname == "") {
		currentHostname = this->ipAddressText->Text;
	}
	else {
		this->ipAddressText->Text = currentHostname;
	}

	config->setHostname(currentHostname);
	this->progressRing->IsActive = true;
	client = new MoonlightClient();
	char ipAddressStr[2048];
	wcstombs_s(NULL, ipAddressStr, currentHostname->Data(), 2047);
	int status = client->Connect(ipAddressStr);
	if (status != 0) {
		this->connectStatus->Text = L"Cannot Connect";
		return;
	}
	if (!client->IsPaired()) {
		char* pin = client->GeneratePIN();
		std::string s_str = std::string(pin);
		std::wstring wid_str = std::wstring(s_str.begin(), s_str.end());
		const wchar_t* w_char = wid_str.c_str();
		Platform::String^ p_string = ref new Platform::String(w_char);
		this->connectStatus->Text = String::Concat(L"Use PIN for pairing and then press Connect Again: ", p_string);
		ProgressRing^ ring = progressRing;
		TextBlock^ text = connectStatus;
		auto t = Concurrency::create_async([]()
			{
				int a = client->Pair();
			});
		this->progressRing->IsActive = false;
	}
	else {

		this->connectStatus->Text = L"Connected";
		this->progressRing->IsActive = false;
		UpdateApps();
	}
	//
}

void moonlight_xbox_dx::MenuPage::UpdateApps() {
	std::vector<MoonlightApplication> apps = client->GetApplications();
	this->appsPanel->Visibility = Windows::UI::Xaml::Visibility::Visible;
	this->appsComboBox->Items->Clear();
	for (auto a : apps) {
		ComboBoxItem^ button = ref new ComboBoxItem();
		std::string s_str = std::string(a.name);
		std::wstring wid_str = std::wstring(s_str.begin(), s_str.end());
		const wchar_t* w_char = wid_str.c_str();
		Platform::String^ p_string = ref new Platform::String(w_char);
		button->Content = p_string;
		button->DataContext = a.id;
		this->appsComboBox->Items->Append(button);
	}
}

void moonlight_xbox_dx::MenuPage::OnAppClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	ComboBoxItem^ item = (ComboBoxItem^)this->appsComboBox->SelectedItem;

	if (Utils::getTextFromBox(WidthTextbox->Text) != "")
	{
		config->setWidth(std::stoi(Utils::getTextFromBox(WidthTextbox->Text)));
	}
	if (Utils::getTextFromBox(HeightTextbox->Text) != "")
	{
		config->setHeight(std::stoi(Utils::getTextFromBox(HeightTextbox->Text)));
	}
	if (Utils::getTextFromBox(FpsTextbox->Text) != "")
	{
		config->setFps(std::stoi(Utils::getTextFromBox(FpsTextbox->Text)));
	}
	if (Utils::getTextFromBox(BitrateTextbox->Text) != "")
	{
		config->setBitrate(std::stoi(Utils::getTextFromBox(FpsTextbox->Text)));
	}

	config->setAppID((int)item->DataContext);
	bool result = this->Frame->Navigate(Windows::UI::Xaml::Interop::TypeName(StreamPage::typeid), config);
	if (result) {
		this->ConnectStatusText->Text = L"OK";
	}
	else {
		this->ConnectStatusText->Text = L"Unable to Navigate onto DirectX Page";
	}
}

void moonlight_xbox_dx::MenuPage::TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}