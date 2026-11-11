#pragma once
    //==========================================================================
    //      include externe
    //==========================================================================
#include <map>
#include <typeindex>
#include <functional>
#include <list>
    //==========================================================================
    //      include interne
    //==========================================================================
#include "../Logger/Logger.h"
#include "bb_event.h"

class IEventCallback {
	public:
		virtual ~IEventCallback() = default;

		void Execute(Event& e) {
			Call(e);
		}
	private:
		virtual void Call(Event& e) = 0;
};

template <typename TOwner, typename TEvent>
class EventCallback: public IEventCallback {
	

	private:
		typedef void (TOwner::*CallbackFunction)(TEvent&);

		TOwner* ownerInstance;
		CallbackFunction callbackFunction;

		virtual void Call(Event& e) override {
			std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
		}

	public:
		EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction) {
			this->ownerInstance = ownerInstance;
			this->callbackFunction = callbackFunction;
		}

		virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
	public:
		EventBus() {
			Logger::Log("EventBus constructor called!");
		}

		~EventBus() {
			Logger::Log("EventBus destructor called!");
		}

		// clears the subscribers list
		void Reset() {
			subscribers.clear();
		}

		/////////////////////////////////////////////////////////////////////////////
		// Subscribe a un event de type <T>
		// Dans notre implementation, un "listener" se "subscribes" a un event
		// Exemple: eventBus->SubscribeToEvent<CollisionEvent>(&Game::onCollision);
		/////////////////////////////////////////////////////////////////////////////
		template <typename TEvent, typename TOwner>
		void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)) {
			if (!subscribers[typeid(TEvent)].get()) {
				subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
			}
			auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
			subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
		}

		/////////////////////////////////////////////////////////////////////////////
		// Emet un event de type <T>
		// Dans notre implementatiion, dès que quelque chose arrive on émet un
		// event et on va éxecuter tout les fonctions en callback des "listeners"
		// Exemple: eventBus->EmitEvent<CollisionEvent>(player, enemy);
		/////////////////////////////////////////////////////////////////////////////
		template <typename TEvent, typename ...TArgs>
		void EmitEvent(TArgs&& ...args) {
			auto handlers = subscribers[typeid(TEvent)].get();
			if (handlers) {
				for (auto it = handlers->begin(); it != handlers->end(); it++) {
					auto handler = it->get();
					TEvent event(std::forward<TArgs>(args)...);
					handler->Execute(event);
				}
			}
		}

	private:
		std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;
};