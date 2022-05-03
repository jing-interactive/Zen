#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/Utilities.h"

#include "AssetManager.h"
#include "MiniConfigImgui.h"
#include "ImGuiColorTextEdit/TextEditor.h"
#include "imgui_markdown/imgui_markdown.h"
#include "IconFontCppHeaders/IconsFontAwesome5.h"

using namespace ci;
using namespace ci::app;
using namespace std;


void LinkCallback(ImGui::MarkdownLinkCallbackData data_);
inline ImGui::MarkdownImageData ImageCallback(ImGui::MarkdownLinkCallbackData data_);

// You can make your own Markdown function with your prefered string container and markdown config.
static ImGui::MarkdownConfig mdConfig{ LinkCallback, NULL, ImageCallback, ICON_FA_LINK, { { NULL, true }, { NULL, true }, { NULL, false } } };

void LinkCallback(ImGui::MarkdownLinkCallbackData data_)
{
    std::string url(data_.link, data_.linkLength);
    if (!data_.isImage)
    {
        launchWebBrowser(Url(url, true));
    }
}

inline ImGui::MarkdownImageData ImageCallback(ImGui::MarkdownLinkCallbackData data_)
{
    // In your application you would load an image based on data_ input. Here we just use the imgui font texture.
    ImTextureID image = ImGui::GetIO().Fonts->TexID;
    ImGui::MarkdownImageData imageData{ true, false, image, ImVec2(40.0f, 20.0f) };
    return imageData;
}

void LoadFonts(float fontSize_ = 12.0f)
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    // Base font
    io.Fonts->AddFontFromFileTTF(getAssetPath(FONT_NAME).string().c_str(), fontSize_);
    // Bold headings H2 and H3
    mdConfig.headingFormats[1].font = io.Fonts->AddFontFromFileTTF(getAssetPath(FONT_BOLD_NAME).string().c_str(), fontSize_);
    mdConfig.headingFormats[2].font = mdConfig.headingFormats[1].font;
    // bold heading H1
    float fontSizeH1 = fontSize_ * 1.1f;
    mdConfig.headingFormats[0].font = io.Fonts->AddFontFromFileTTF(getAssetPath(FONT_BOLD_NAME).string().c_str(), fontSizeH1);
}

void Markdown(const std::string& markdown_)
{
    // fonts for, respectively, headings H1, H2, H3 and beyond
    ImGui::Markdown(markdown_.c_str(), markdown_.length(), mdConfig);
}

struct ZenApp : public App
{
    TextEditor editor;

    void setupEditor()
    {
        // Load Fonts
// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
        ///////////////////////////////////////////////////////////////////////
        // TEXT EDITOR SAMPLE
        auto lang = TextEditor::LanguageDefinition::CPlusPlus();

        // set your own known preprocessor symbols...
        if (0)
        {
            static const char* ppnames[] = { "NULL", "PM_REMOVE",
                "ZeroMemory", "DXGI_SWAP_EFFECT_DISCARD", "D3D_FEATURE_LEVEL", "D3D_DRIVER_TYPE_HARDWARE", "WINAPI","D3D11_SDK_VERSION", "assert" };
            // ... and their corresponding values
            static const char* ppvalues[] = {
                "#define NULL ((void*)0)",
                "#define PM_REMOVE (0x0001)",
                "Microsoft's own memory zapper function\n(which is a macro actually)\nvoid ZeroMemory(\n\t[in] PVOID  Destination,\n\t[in] SIZE_T Length\n); ",
                "enum DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD = 0",
                "enum D3D_FEATURE_LEVEL",
                "enum D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE  = ( D3D_DRIVER_TYPE_UNKNOWN + 1 )",
                "#define WINAPI __stdcall",
                "#define D3D11_SDK_VERSION (7)",
                " #define assert(expression) (void)(                                                  \n"
                "    (!!(expression)) ||                                                              \n"
                "    (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \n"
                " )"
            };

            for (int i = 0; i < sizeof(ppnames) / sizeof(ppnames[0]); ++i)
            {
                TextEditor::Identifier id;
                id.mDeclaration = ppvalues[i];
                lang.mPreprocIdentifiers.insert(std::make_pair(std::string(ppnames[i]), id));
            }
        }

        // set your own identifiers
        if (0)
        {
            static const char* identifiers[] = {
                "HWND", "HRESULT", "LPRESULT","D3D11_RENDER_TARGET_VIEW_DESC", "DXGI_SWAP_CHAIN_DESC","MSG","LRESULT","WPARAM", "LPARAM","UINT","LPVOID",
                "ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
                "ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
                "IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "TextEditor" };
            static const char* idecls[] =
            {
                "typedef HWND_* HWND", "typedef long HRESULT", "typedef long* LPRESULT", "struct D3D11_RENDER_TARGET_VIEW_DESC", "struct DXGI_SWAP_CHAIN_DESC",
                "typedef tagMSG MSG\n * Message structure","typedef LONG_PTR LRESULT","WPARAM", "LPARAM","UINT","LPVOID",
                "ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
                "ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
                "IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "class TextEditor" };
            for (int i = 0; i < sizeof(identifiers) / sizeof(identifiers[0]); ++i)
            {
                TextEditor::Identifier id;
                id.mDeclaration = std::string(idecls[i]);
                lang.mIdentifiers.insert(std::make_pair(std::string(identifiers[i]), id));
            }
            //editor.SetPalette(TextEditor::GetLightPalette());
        }
        editor.SetLanguageDefinition(lang);

        // error markers
        if (0)
        {
            TextEditor::ErrorMarkers markers;
            markers.insert(std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
            markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
            editor.SetErrorMarkers(markers);
        }

        //"breakpoint" markers
        if (0)
        {
            TextEditor::Breakpoints bpts;
            bpts.insert(24);
            bpts.insert(47);
            editor.SetBreakpoints(bpts);
        }

        auto str = am::str(FILE_NAME);
        editor.SetText(str);
    }

    void setup() override
    {
        log::makeLogger<log::LoggerFileRotating>(fs::path(), "IG.%Y.%m.%d.log");
        createConfigImgui();

        setupEditor();
        //LoadFonts();

        getWindow()->getSignalKeyUp().connect([&](KeyEvent& event) {
            if (event.getCode() == KeyEvent::KEY_ESCAPE) quit();
        });

        getWindow()->getSignalResize().connect([&]{
            APP_WIDTH = getWindowWidth();
            APP_HEIGHT = getWindowHeight();
        });

        getSignalUpdate().connect([&] {
            auto cpos = editor.GetCursorPosition();
            ImGui::Begin("Text Editor Demo", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
            ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Save"))
                    {
                        auto textToSave = editor.GetText();
                        /// save text....
                    }
                    if (ImGui::MenuItem("Quit", "Alt-F4"))
                    {
                        quit();
                        return;
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit"))
                {
                    bool ro = editor.IsReadOnly();
                    if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                        editor.SetReadOnly(ro);
                    ImGui::Separator();

                    if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
                        editor.Undo();
                    if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
                        editor.Redo();

                    ImGui::Separator();

                    if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
                        editor.Copy();
                    if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
                        editor.Cut();
                    if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
                        editor.Delete();
                    if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
                        editor.Paste();

                    ImGui::Separator();

                    if (ImGui::MenuItem("Select all", nullptr, nullptr))
                        editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("View"))
                {
                    if (ImGui::MenuItem("Dark palette"))
                        editor.SetPalette(TextEditor::GetDarkPalette());
                    if (ImGui::MenuItem("Light palette"))
                        editor.SetPalette(TextEditor::GetLightPalette());
                    if (ImGui::MenuItem("Retro blue palette"))
                        editor.SetPalette(TextEditor::GetRetroBluePalette());
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Help"))
                {
                    if (ImGui::MenuItem("About"))
                    {
                    }

                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
                editor.IsOverwrite() ? "Ovr" : "Ins",
                editor.CanUndo() ? "*" : " ",
                editor.GetLanguageDefinition().mName.c_str(), FILE_NAME.c_str());

            editor.Render("TextEditor");

            ImGui::End();

            ImGui::Begin("Preview");
            Markdown(editor.GetText());
            ImGui::End();
        });

        getSignalCleanup().connect([&] { writeConfig(); });

        getWindow()->getSignalDraw().connect([&] {
            gl::clear();
        });
    }
};

CINDER_APP(ZenApp, RendererGl, [](App::Settings* settings) {
    readConfig();
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setMultiTouchEnabled(false);
})
