#include "Common.h"
#include "String.h"
#include "Result.h"
#include "Queue.h"
#include "GL.h"

namespace boxer {
	enum class EventType : U32 {
		// Application
		EVENT_APP_CREATE,
		EVENT_APP_UPDATE,
		EVENT_APP_DESTROY,
		// Window
		EVENT_WINDOW_RESIZE,
		// Keyboard
		EVENT_KEY_PRESSED,
		EVENT_KEY_RELEASED,
		// Mouse
		EVENT_MOUSE_PRESSED,
		EVENT_MOUSE_RELEASED,
	};
	
	struct Event {
		EventType _Type;
		// TODO(NeGate): Implement this!
	};

	class Application {
	private:
		// An OS-specific handle (can literally be anything)
		Handle _Handle;
		
		String _Title;
		U32 _Width, _Height;

		Queue<Event> m_Queue;
	public:
		Application(String title, U32 width, U32 height);

		void Launch();
		void Close();
	};
}