#include <systems/rhythm_system.h>

using namespace Halley;

class RhythmSystem final : public RhythmSystemBase<RhythmSystem> {
public:
	void init()
	{
		getRhythmService().start(*getAPI().audio);
	}
	
	void update(Time t)
    {
		getRhythmService().update(t);

		if (lastBeat != getRhythmService().getCurrentBeat()) {
			lastBeat = getRhythmService().getCurrentBeat();
			onNewBeat(lastBeat);
		}
		
		for (auto& e: mainFamily) {
			e.rhythmArea.elapsed += float(t);

			float t = e.rhythmArea.elapsed / e.rhythmArea.totalTime;
			if (t > 1.25f) {
				getWorld().destroyEntity(e.entityId);
			}
		}
	}

private:
	int lastBeat = -1;

	void createBeatMarker(int id, BlacksmithActions action)
	{
		Vector2f pos;
		switch (action) {
		case BlacksmithActions::Anvil:
			pos = Vector2f(192, 166);
			break;
		case BlacksmithActions::Bucket:
			pos = Vector2f(133, 108);
			break;
		case BlacksmithActions::Furnace:
			pos = Vector2f(192, 50);
			break;
		case BlacksmithActions::Love:
			pos = Vector2f(251, 108);
			break;
		}
		
		const float beatLen = 60.0f / getRhythmService().getBPM();
		getWorld().createEntity()
			.addComponent(PositionComponent(pos))
			.addComponent(RhythmAreaComponent(id, -0.5f * beatLen, 1.5f * beatLen));
	}

	void onNewBeat(int curBeat)
	{
		// Draw next beat
		int lookAhead = 2;
		Logger::logInfo("On beat " + toString(curBeat));
		auto action = getRhythmService().getActionAtBeat(curBeat + lookAhead);

		if (action != BlacksmithActions::Idle) {
			createBeatMarker(curBeat + lookAhead, action);
		}
	}
};

REGISTER_SYSTEM(RhythmSystem)

