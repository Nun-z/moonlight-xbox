//
// MenuPage.xaml.h
// Declaration of the MenuPage class
//

#pragma once

#include "MenuPage.g.h"

namespace moonlight_xbox_dx
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class MenuPage sealed
	{
	public:
		MenuPage();
	private:
		void ConnectButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnAppClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void UpdateApps();
		void TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnLoadClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
