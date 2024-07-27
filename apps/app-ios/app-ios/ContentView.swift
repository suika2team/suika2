import SwiftUI
import MetalKit
import UniformTypeIdentifiers

@main
struct app_iosApp: App {
    var body: some Scene {
        WindowGroup {
            ContentView()
        }
    }
}

struct ContentView: View {
    @State static var showingPopup = false
    @State static var popupText = ""

    var body: some View {
        ZStack {
            if ContentView.showingPopup {
                PopupView(
                    isPresent: ContentView.$showingPopup,
                    popupText: ContentView.$popupText
                )
            }
            NavigationView {
                List {
                    NavigationLink(destination: ProjectView()) {
                        Text("プロジェクト")
                    }
                    NavigationLink(destination: EditorView()) {
                        Text("エディタ")
                    }
                    NavigationLink(destination: RunView()) {
                        Text("実行")
                    }
                }
            }
        }
    }

    static func alert(text: String) {
        ContentView.popupText = text
        ContentView.showingPopup = true
    }
}

struct PopupView: View {
    @Binding var isPresent: Bool
    @Binding var popupText: String

    var body: some View {
        VStack(spacing: 12) {
            Text("エラー")
            .font(Font.system(size: 18).bold())

            Image("icon")
            .resizable()
            .frame(width: 80, height: 80)

            Text("\(popupText)")
            .font(Font.system(size: 18))

            Button(action: {
                withAnimation {
                    isPresent = false
                }
            }, label: {
                Text("閉じる")
            })
        }
        .frame(width: 280, alignment: .center)
        .padding()
        .background(Color.white)
        .cornerRadius(12)
    }
}

struct ProjectView: View {
    @State var importerPresented = false
    @State var projectPath = ""

    var body: some View {
        List {
            // Open project button.
            Button() {
                importerPresented.toggle()
            } label: {
                Text("プロジェクトを開く")
            }
            .fileImporter(isPresented: $importerPresented,
                          allowedContentTypes: [.folder],
                          allowsMultipleSelection: false) { result in
                switch result {
                case .success(let urls):
                    guard let url = urls.first else {
                        return
                    }
                    let access = url.startAccessingSecurityScopedResource()
                    if access {
                        //url.stopAccessingSecurityScopedResource()
                        projectPath = url.path().removingPercentEncoding ?? ""
                        Renderer.projectPath = projectPath
                        onOpenProject()
                    }
                case .failure(let error):
                    print(error.localizedDescription)
                }
            }

            // Indicator of the current project.
            Text("開いているプロジェクト:")
            Text("\(projectPath)")
            
            // New project button.
            Button() {
                importerPresented.toggle()
            } label: {
                
                Text("新規プロジェクト")
            }
            .fileImporter(isPresented: $importerPresented,
                          allowedContentTypes: [.folder],
                          allowsMultipleSelection: false) { result in
                switch result {
                case .success(let urls):
                    guard let url = urls.first else { return }
                    let access = url.startAccessingSecurityScopedResource()
                    if access {
                        projectPath = url.path().removingPercentEncoding ?? ""
                        Renderer.projectPath = projectPath
                        onNewProject()
                    }
                case .failure(let error):
                    print(error.localizedDescription)
                }
            }
            
            // Export for iOS button.
            Button(action: onExportIOS) {
                Text("iOS アプリをエクスポート")
            }
            
            // Export for Android button.
            Button(action: onExportAndroid) {
                Text("Non-iOS 電話のアプリをエクスポート")
            }
            
            // Export for macOS button.
            Button(action: onExportMac) {
                Text("macOS アプリをエクスポート")
            }
            
            // Export for Windows button.
            Button(action: onExportWindows) {
                Text("PC アプリをエクスポート")
            }
            
            // Export for Web button.
            Button(action: onExportWeb) {
                Text("Web アプリをエクスポート")
            }
        }
    }
    
    func onOpenProject() {
    }
    
    func onNewProject() {
    }
    
    func onExportIOS() {
    }
    
    func onExportAndroid() {
    }
    
    func onExportMac() {
    }
    
    func onExportWindows() {
    }
    
    func onExportWeb() {
    }
}

struct EditorView: View {
    @State var selectedCommand = ""
    @State var textContent = ""

    let commands = [
        "commandMessage",
        "commandLine",
        "commandBackground",
        "commandCharacter"
    ]

    var body: some View {
        VStack {
            HStack {
                Button(action: {}) {
                    Text("新規")
                }
                Button(action: {}) {
                    Text("開く")
                }
                Button(action: onSaveFile) {
                    Text("保存")
                }
            }
            HStack {
                Picker(selection: $selectedCommand, label: Text("コマンド"), content: {
                    ForEach(commands, id:\.self) { value in
                        Text("\(value)").tag(value)
                    }
                })
                .pickerStyle(.menu)
                Button(action: {
                    onInsertCommand(command: selectedCommand)
                }) {
                    Text("追加")
                }
            }
            TextEditor(text: $textContent)
        }
    }
    
    func onSaveFile() {
    }
    
    func onLoadFile() {
    }

    func onInsertCommand(command: String) {
    }
}

struct RunView: View {
    var gameView = GameViewRepresentable()

    var body: some View {
        VStack {
            Button(action: onRestart) {
                Text("スタート")
            }
            gameView
        }
    }

    func onRestart() {
        Renderer.restart()
    }
}

#Preview {
    ContentView()
}

#if os(iOS)
private typealias ViewRepresentable = UIViewRepresentable
#elseif os(macOS)
private typealias ViewRepresentable = NSViewRepresentable
#endif

struct GameViewRepresentable: ViewRepresentable {
    #if os(macOS)
    typealias NSViewType = GameView
    #else
    typealias UIViewType = GameView
    #endif

    var view: GameView
    var coordinator: Renderer

    init() {
        view = GameView()
        coordinator = Renderer(view: view)
        view.delegate = coordinator
    }

    #if os(iOS)
    func makeCoordinator() -> Renderer { return coordinator }
    func makeUIView(context: Context) -> MTKView { return view }
    func updateUIView(_ uiView: GameView, context: Context) { }
    #endif

    #if os(macOS)
    func makeCoordinator() -> Renderer { return coordinator }
    func makeNSView(context: Context) -> GameView { return view }
    func updateNSView(_ nsView: GameView, context: Context) { }
    #endif
}

class GameView: MTKView {
    #if os(iOS)
    func touchesBegan(_: Set<UITouch>, with: UIEvent) {
        Renderer.touchesBegan(_, with: with)
    }
    func touchesMoved(_: Set<UITouch>, with: UIEvent) {
        Renderer.touchesMoved(_, with: with)
    }
    func touchesEnded(_: Set<UITouch>, with: UIEvent) {
        Renderer.touchesEnded(_, with: with)
    }
    #endif

    #if os(macOS)
    override func mouseDown(with event: NSEvent) {
        Renderer.mouseDown(with: event)
    }
    override func mouseUp(with: NSEvent) {
        Renderer.mouseUp(with: with)
    }
    override func mouseDragged(with: NSEvent) {
        Renderer.mouseDragged(with: with)
    }
    override func scrollWheel(with: NSEvent) {
        Renderer.scrollWheel(with: with)
    }
    #endif
}

struct TextFile: FileDocument {
    static var readableContentTypes = [UTType.plainText]

    var text = ""

    init(initialText: String = "") {
        text = initialText
    }

    init(configuration: ReadConfiguration) throws {
        if let data = configuration.file.regularFileContents {
            text = String(decoding: data, as: UTF8.self)
        }
    }

    func fileWrapper(configuration: WriteConfiguration) throws -> FileWrapper {
        let data = Data(text.utf8)
        return FileWrapper(regularFileWithContents: data)
    }
}
