//
//  ContentView.swift
//  exporter-macos
//
//  Created by tabata on 2024/08/02.
//

import SwiftUI

@main
struct exporter_macosApp: App {
    var body: some Scene {
        WindowGroup {
            ContentView()
        }
    }
}

struct ContentView: View {
    @State var importerPresented = false
    @State var projectPath = ""
    @State var disableIOS = true
    @State var disableAndroid = true
    @State var disableWindows = true
    @State var disableMacOS = true
    @State var disableUnity = true
    @State var disableWeb = true

    var body: some View {
        VStack {
            Text("🎮")
            .font(Font.system(size: 50).bold())
            Text("OpenNovel Exporter")
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
                    guard let url = urls.first else { return }
                    let access = url.startAccessingSecurityScopedResource()
                    if access {
                        projectPath = url.path().removingPercentEncoding ?? ""
                        disableIOS = false
                        disableAndroid = false
                        disableWindows = false
                        disableMacOS = false
                        disableUnity = false
                        disableWeb = false
                    }
                case .failure(let error):
                    print(error.localizedDescription)
                }
            }
            Text("\(projectPath)")
            List {
                var ios = Button(action: onExportForIOS) {
                    Text("iOS 用にエクスポートする")
                }
                .disabled(disableIOS)
                var android = Button(action: onExportForAndroid) {
                    Text("Android 用にエクスポートする")
                }
                .disabled(disableAndroid)
                var windows = Button(action: onExportForWindows) {
                    Text("Windows 用にエクスポートする")
                }
                .disabled(disableWindows)
                var macos = Button(action: onExportForMacOS) {
                    Text("macOS 用にエクスポートする")
                }
                .disabled(disableMacOS)
                var unity = Button(action: onExportForUnity) {
                    Text("Unity 用にエクスポートする")
                }
                .disabled(disableUnity)
                var web = Button(action: onExportForWeb) {
                    Text("Web 用にエクスポートする")
                }
                .disabled(disableWeb)
            }
            .padding()
        }
    }

    func onExportForIOS() {
    }

    func onExportForAndroid() {
    }

    func onExportForWindows() {
    }

    func onExportForMacOS() {
    }

    func onExportForUnity() {
    }

    func onExportForWeb() {
    }
}

#Preview {
    ContentView()
}
