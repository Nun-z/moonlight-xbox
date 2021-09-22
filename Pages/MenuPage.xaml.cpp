//
// MenuPage.xaml.cpp
// Implementation of the MenuPage class
//

#include "pch.h"
#include "MenuPage.xaml.h"
#include "StreamPage.xaml.h"
#include "UserSelections.h"
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

StreamConfiguration^ config;
MoonlightClient* client;
MenuPage::MenuPage()
{
	InitializeComponent();
	concurrency::create_task([] {
		UserSelections::initalizeUserFields();
		});
}


void moonlight_xbox_dx::MenuPage::ConnectButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	config = ref new StreamConfiguration();
	Platform::String ^ipAddress = this->ipAddressText->Text;
	config->hostname = ipAddress;
	this->progressRing->IsActive = true;
	client = new MoonlightClient();
	char ipAddressStr[2048];
	wcstombs_s(NULL, ipAddressStr, ipAddress->Data(), 2047);
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
		this->connectStatus->Text =String::Concat(L"Use PIN for pairing and then press Connect Again: ", p_string);
		ProgressRing ^ring = progressRing;
		TextBlock ^text = connectStatus;
		auto t = Concurrency::create_async([]()
		{
				int a = client->Pair();
		});
		this->progressRing->IsActive = false;
	}
	else {
		Platform::String^ ipAddress = this->ipAddressText->Text;
		this->progressRing->IsActive = true;
		MoonlightClient* client = MoonlightClient::GetInstance();
		char ipAddressStr[2048];
		wcstombs_s(NULL, ipAddressStr, ipAddress->Data(), 2047);
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
					MoonlightClient* client = MoonlightClient::GetInstance();
					int a = client->Pair();
				});
			this->progressRing->IsActive = false;
		}
		else {
			this->connectStatus->Text = L"Connected";
			this->progressRing->IsActive = false;
			UpdateApps();
		}
	}
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
	std::string width = Utils::getTextFromBox(WidthTextbox->Text);
	std::string height = Utils::getTextFromBox(HeightTextbox->Text);
	std::string fps = Utils::getTextFromBox(FpsTextbox->Text);
	std::string bitrate = Utils::getTextFromBox(BitrateTextbox->Text);

	UserSelections::setFps(std::stoi(fps));
	UserSelections::setBitrate(std::stoi(bitrate));
	UserSelections::setWidth(std::stoi(width));
	UserSelections::setHeight(std::stoi(height));

void moonlight_xbox_dx::MenuPage::OnAppClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	ComboBoxItem^ item = (ComboBoxItem^) this->appsComboBox->SelectedItem;
	config->appID = (int)item->DataContext;
	config->width = 1280;
	config->height = 720;
	bool result = this->Frame->Navigate(Windows::UI::Xaml::Interop::TypeName(StreamPage::typeid),config);
	if (result) {
		this->ConnectStatusText->Text = L"OK";
	}
	else {
		ResolutionStatus->Text = "";

		ComboBoxItem^ item = (ComboBoxItem^)this->appsComboBox->SelectedItem;
		MoonlightClient::GetInstance()->SetAppID((int)item->DataContext);
		MoonlightClient::GetInstance()->SetSoftwareEncoder(/*this->UseSoftwareEncoder->IsChecked->Value*/false);
		bool result = this->Frame->Navigate(Windows::UI::Xaml::Interop::TypeName(StreamPage::typeid));
		if (result) {
			this->ConnectStatusText->Text = L"OK";
		}
		else {
			this->ConnectStatusText->Text = L"Unable to Navigate onto DirectX Page";
		}
	}
}

void moonlight_xbox_dx::MenuPage::TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}

void moonlight_xbox_dx::MenuPage::OnLoadClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Platform::String^ currentIpAddress = Utils::getPlatformString(UserSelections::getIpAddress());
	if (currentIpAddress != "") {
		ipAddressText->Text = currentIpAddress;
	}
	WidthTextbox->Text = Utils::getPlatformString(std::to_string(UserSelections::getWidth()));
	HeightTextbox->Text = Utils::getPlatformString(std::to_string(UserSelections::getHeight()));
	FpsTextbox->Text = Utils::getPlatformString(std::to_string(UserSelections::getFps()));
	BitrateTextbox->Text = Utils::getPlatformString(std::to_string(UserSelections::getBitrate()));
}