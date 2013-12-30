#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"

#include <algorithm>

MessageHub::MessageHub() {
}

MessageHub::~MessageHub() {
	this->destroy();
}

void MessageHub::init() {
	this->currentlyAcceptingMessageCacheRef = &(this->frontMessageCache);
	this->currentlyDrainingMessageCacheRef = nullptr;
}

void MessageHub::destroy() {
}

void MessageHub::SendPointcastMessage(const Message* const message, ObjectIdType receiverId, ObjectIdType senderId /* = OBJECT_ID_INVALID */) {
	// TODO [Cleanup] Too many allocations here. Pools maybe? Or better yet, define messages as const, so that we can just send the same message to all of them (reference counted), somehow
	Message* messageCopy = new Message(*message);

	messageCopy->setReceiverId(receiverId);
	messageCopy->setSenderId(senderId);

	this->currentlyAcceptingMessageCacheRef->PushMessageForReceipientId(messageCopy, receiverId);
}

void MessageHub::SendMulticastMessage(const Message* const message, ObjectIdType senderId /* = OBJECT_ID_INVALID */) {
	unsigned int numSubscribers = this->subscribersForMessageType[message->GetMessageType()].size();
	for (unsigned int i = 0; i < numSubscribers; ++i) {
		this->SendPointcastMessage(message, this->subscribersForMessageType[message->GetMessageType()][i], senderId);
	}
}

void MessageHub::SubscribeToMessageType(MessageType messageType, ObjectIdType subscriberId) {
	auto it = std::find(this->subscribersForMessageType[messageType].begin(), this->subscribersForMessageType[messageType].end(), subscriberId);
	if (it == this->subscribersForMessageType[messageType].end()) {
		this->subscribersForMessageType[messageType].push_back(subscriberId);
	} else {
		FRAMEWORK->GetLogger()->dbglog("Duplicate subscription for messageType:%d by object id: %d", messageType, subscriberId);
	}
}

void MessageHub::UnsubscribeToMessageType(MessageType messageType, ObjectIdType subscriberId) {
	auto it = std::find(this->subscribersForMessageType[messageType].begin(), this->subscribersForMessageType[messageType].end(), subscriberId);
	if (it != this->subscribersForMessageType[messageType].end()) {
		this->subscribersForMessageType[messageType].erase(it);
	} else {
		FRAMEWORK->GetLogger()->dbglog("Warning: Trying to unsubscribe for unfound subscription for messageType:%d by object id: %d", messageType, subscriberId);
	}
}

Message* MessageHub::RetrieveNextMessage(ObjectIdType id) {
	return this->currentlyDrainingMessageCacheRef->PopMessageForReceipientId(id);
}


void MessageHub::drainMessages() {
	this->currentlyAcceptingMessageCacheRef = &this->backMessageCache;
	this->currentlyDrainingMessageCacheRef = &this->frontMessageCache;
	this->drainMessageCache_internal();

	// Switching MessageCaches

	this->currentlyAcceptingMessageCacheRef = &this->frontMessageCache;
	this->currentlyDrainingMessageCacheRef = &this->backMessageCache;
	this->drainMessageCache_internal();

	this->currentlyDrainingMessageCacheRef = nullptr;
}

void MessageHub::drainMessageCache_internal() {
	ObjectIdType receipientId = OBJECT_ID_INVALID;
	for (auto objectId_it = this->currentlyDrainingMessageCacheRef->objectIdsWithPendingMessages.begin();
			objectId_it != this->currentlyDrainingMessageCacheRef->objectIdsWithPendingMessages.end(); ++objectId_it) {

		if (objectId_it->second) {
			receipientId = objectId_it->first;

			Object* receipient = ObjectRegistry::GetObjectById(receipientId);
			if (receipient != nullptr) {
				receipient->HandleMessages();
			}
		}
		this->currentlyDrainingMessageCacheRef->ClearMessagesForReceipientId(receipientId);
	}
	this->currentlyDrainingMessageCacheRef->objectIdsWithPendingMessages.clear();
}
