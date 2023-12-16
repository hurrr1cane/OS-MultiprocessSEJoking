#pragma once

#include <Windows.h>
#include <msclr\marshal_cppstd.h>
#include <vector>

namespace OSLab09Client {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace System::IO;
	using namespace msclr::interop;
	using namespace System::Text;
	using namespace System::Net;
	using namespace System::Net::Sockets;
	using namespace System::Text;
	using namespace System::Threading;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			notifyIcon1->Icon = gcnew System::Drawing::Icon("meme_this_is_fine_dog.ico"); // Provide the path to your icon file
			notifyIcon1->Visible = true;
			backgroundWorker1->WorkerSupportsCancellation = true;
			StartBackgroundWorker();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::CheckBox^ subscriptionCheckbox;
	protected:
	private: System::Windows::Forms::Button^ saveButton;
	private: System::Windows::Forms::NotifyIcon^ notifyIcon1;
	private: System::Windows::Forms::ContextMenuStrip^ contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ exitToolStripMenuItem;
	private: System::ComponentModel::BackgroundWorker^ backgroundWorker1;

	private: System::Windows::Forms::TextBox^ jokesTextBox;
	private: System::Windows::Forms::Label^ label1;

	private: System::ComponentModel::IContainer^ components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->subscriptionCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->saveButton = (gcnew System::Windows::Forms::Button());
			this->notifyIcon1 = (gcnew System::Windows::Forms::NotifyIcon(this->components));
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->jokesTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->contextMenuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// subscriptionCheckbox
			// 
			this->subscriptionCheckbox->AutoSize = true;
			this->subscriptionCheckbox->Checked = true;
			this->subscriptionCheckbox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->subscriptionCheckbox->Location = System::Drawing::Point(74, 47);
			this->subscriptionCheckbox->Name = L"subscriptionCheckbox";
			this->subscriptionCheckbox->Size = System::Drawing::Size(152, 20);
			this->subscriptionCheckbox->TabIndex = 0;
			this->subscriptionCheckbox->Text = L"Subscribe to memes";
			this->subscriptionCheckbox->UseVisualStyleBackColor = true;
			// 
			// saveButton
			// 
			this->saveButton->Location = System::Drawing::Point(530, 35);
			this->saveButton->Name = L"saveButton";
			this->saveButton->Size = System::Drawing::Size(164, 42);
			this->saveButton->TabIndex = 1;
			this->saveButton->Text = L"Save";
			this->saveButton->UseVisualStyleBackColor = true;
			this->saveButton->Click += gcnew System::EventHandler(this, &MainForm::saveButton_Click);
			// 
			// notifyIcon1
			// 
			this->notifyIcon1->ContextMenuStrip = this->contextMenuStrip1;
			this->notifyIcon1->Text = L"SE Memes";
			this->notifyIcon1->Visible = true;
			this->notifyIcon1->BalloonTipClicked += gcnew System::EventHandler(this, &MainForm::notifyIcon1_BalloonTipClicked);
			this->notifyIcon1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::notifyIcon1_MouseClick);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->openToolStripMenuItem,
					this->exitToolStripMenuItem
			});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(115, 52);
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(114, 24);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::openToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(114, 24);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::exitToolStripMenuItem_Click_1);
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainForm::backgroundWorker1_DoWork);
			// 
			// jokesTextBox
			// 
			this->jokesTextBox->Location = System::Drawing::Point(74, 118);
			this->jokesTextBox->Multiline = true;
			this->jokesTextBox->Name = L"jokesTextBox";
			this->jokesTextBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->jokesTextBox->Size = System::Drawing::Size(620, 305);
			this->jokesTextBox->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(74, 96);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(86, 16);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Recent jokes";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(763, 466);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->jokesTextBox);
			this->Controls->Add(this->saveButton);
			this->Controls->Add(this->subscriptionCheckbox);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MainForm";
			this->Text = L"SE Memes";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainForm::MainForm_FormClosed);
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->contextMenuStrip1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e) {
	}

	private: System::Void MainForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
		if (e->CloseReason == CloseReason::UserClosing) {
			e->Cancel = true;  // Cancel the close event
			this->Hide();  // Hide the form
		}
		else {
			this->Close();
		}
	}

	private: System::Void openToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		this->Show();
		this->WindowState = FormWindowState::Normal;
	}

	private: System::Void backgroundWorker1_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e) {
	}

	private:
		TcpClient^ client;
		NetworkStream^ stream;

		System::Void backgroundWorker1_DoWork(System::Object^ sender, DoWorkEventArgs^ e) {
			array<unsigned char>^ buffer = gcnew array<unsigned char>(512);

			try {
				String^ serverIp = "127.0.0.1";
				int serverPort = 27015;

				client = gcnew TcpClient(serverIp, serverPort);
				stream = client->GetStream();

				while (true) {
					if (backgroundWorker1->CancellationPending) {
						e->Cancel = true;
						return;
					}

					try {
						int bytesRead = stream->Read(buffer, 0, buffer->Length);
						if (bytesRead > 0) {
							String^ message = Encoding::ASCII->GetString(buffer, 0, bytesRead);
							notifyIcon1->ShowBalloonTip(3000, "A new joke has arrived", message, System::Windows::Forms::ToolTipIcon::Info);

							jokesTextBox->Text = message + Environment::NewLine + Environment::NewLine + jokesTextBox->Text;
						}
					}
					catch (Exception^ ex) {
						Console::WriteLine("Exception: " + ex->Message);
					}

					Thread::Sleep(100);
				}
			}
			catch (Exception^ ex) {
				Console::WriteLine("Exception: " + ex->Message);
			}
		}
	private: System::Void StartBackgroundWorker() {
		if (!backgroundWorker1->IsBusy) {
			backgroundWorker1->RunWorkerAsync();
		}
	}

	private: System::Void StopBackgroundWorker() {
		if (backgroundWorker1->IsBusy) {
			backgroundWorker1->CancelAsync();
		}
	}

	private: System::Void MainForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e) {
		StopBackgroundWorker();
	}

	private: System::Void notifyIcon1_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		if (e->Button == System::Windows::Forms::MouseButtons::Left) {
			this->Show();
		}
	}
	private: System::Void exitToolStripMenuItem_Click_1(System::Object^ sender, System::EventArgs^ e) {
		Application::Exit();
	}
	private: System::Void notifyIcon1_BalloonTipClicked(System::Object^ sender, System::EventArgs^ e) {
		this->Show();
		this->WindowState = FormWindowState::Normal;
	}
	private: System::Void saveButton_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!subscriptionCheckbox->Checked) {
			StopBackgroundWorker();
		}
		else {
			StartBackgroundWorker();
		}
	}

	};
}
