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

	std::string savedHostname = Utils::getTextFromBox(currentHostname);
	std::string enteredHostname = Utils::getTextFromBox(ipAddressText->Text);

	if ((savedHostname == "") ||
	((enteredHostname != "") && (savedHostname != enteredHostname))) {
		currentHostname = this->ipAddressText->Text;
	}
	else {
		this->ipAddressText->Text = currentHostname;
	}
	if (this->ipAddressText->Text != "") {
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

	if (Utils::getTextFromBox(WidthTextbox->Text) != "") {
		int width = std::stoi(Utils::getTextFromBox(WidthTextbox->Text));
		concurrency::create_task([width] { config->setWidth(width); } );
	}
	if (Utils::getTextFromBox(HeightTextbox->Text) != "") {
		int height = std::stoi(Utils::getTextFromBox(HeightTextbox->Text));
		concurrency::create_task([height] { config->setHeight(height); });
	}
	if (Utils::getTextFromBox(FpsTextbox->Text) != "") {
		int fps = std::stoi(Utils::getTextFromBox(FpsTextbox->Text));
		concurrency::create_task([fps] { config->setFps(fps); });
	}
	if (Utils::getTextFromBox(BitrateTextbox->Text) != "") {
		int bitrate = std::stoi(Utils::getTextFromBox(BitrateTextbox->Text));
		concurrency::create_task([bitrate] { config->setBitrate(bitrate); });
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

void moonlight_xbox_dx::MenuPage::TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {

}

void moonlight_xbox_dx::MenuPage::SaveButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) 
{
	try {
		Platform::String^ hostname = ipAddressText->Text;
		concurrency::create_task([hostname] { config->setHostname(hostname); });

		int width = stoi(Utils::getTextFromBox(WidthTextbox->Text));
		concurrency::create_task([width] { config->setWidth(width); });

		int height = stoi(Utils::getTextFromBox(HeightTextbox->Text));
		concurrency::create_task([height] { config->setHeight(height); });

		int fps = stoi(Utils::getTextFromBox(FpsTextbox->Text));
		concurrency::create_task([fps] { config->setFps(fps); });

		int bitrate = stoi(Utils::getTextFromBox(BitrateTextbox->Text));
		concurrency::create_task([bitrate] { config->setBitrate(bitrate); });

		concurrency::create_task([] { config->serializeConfiguration(); });
		ErrorText->Text = "Save was performed successfully.";
	}
	catch (std::exception e) {
		ErrorText->Text = "Save failed.  One or more fields is empty or malformed.";
	}
}