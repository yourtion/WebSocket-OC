Pod::Spec.new do |s|

  s.name         = "WebSocket"
  s.version      = "0.0.1"
  s.summary      = "Simple WebSocket in Objective-C"

  s.description  = <<-DESC
                   Simple WebSocket in Objective-C .
                   DESC

  s.homepage     = "https://github.com/yourtion/WebSocket-OC"
  s.license      = "MIT"
  s.author       = { "Yourtion" => "yourtion@gmail.com" }
  s.source       = { :git => "https://github.com/yourtion/WebSocket-OC.git", :tag => s.version }
  s.source_files  = "WebSocket"
  s.requires_arc = true
  s.ios.deployment_target = '8.0'
  # s.osx.deployment_target = '10.9'
  # s.watchos.deployment_target = '2.0'
  # s.tvos.deployment_target = '9.0'

end
