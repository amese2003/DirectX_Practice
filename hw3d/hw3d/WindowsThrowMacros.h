#pragma once
#define CUSTOM_EXCEPT( hr ) Window::HrException( __LINE__,__FILE__,hr )
#define CUSTOM_LAST_EXCEPT() Window::HrException( __LINE__,__FILE__,GetLastError() )
#define CUSTOM_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )