#include "Common.h"
#include "String.h"

namespace boxer {
	class Window {
	private:
		// An OS-specific handle (can literally be anything)
		Handle _Handle;
		
		String _Title;
		U32 _Width, _Height;
		bool _ShouldClose;
	public:
		Window(String title, U32 width, U32 height);
		~Window();

		void Show();
		void Update();
		void Close();

		inline bool ShouldClose() const { return _ShouldClose; }
	};
}