/*
This file is part of Telegram Desktop Matzu Mod.
*/
#include "core/matzu_settings.h"
#include "core/core_settings.h"

#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QDir>

namespace Matzu {
namespace {

constexpr auto kFileName = "tdata/matzu_config.json";

QString ConfigFilePath() {
	return cWorkingDir() + kFileName;
}

} // namespace

Settings &Settings::Instance() {
	static Settings instance;
	return instance;
}

Settings::Settings() {
	load();
}

bool Settings::proMode() const {
	return _proMode.current();
}

void Settings::setProMode(bool enabled) {
	if (_proMode.current() != enabled) {
		_proMode = enabled;
		save();
	}
}

rpl::producer<bool> Settings::proModeValue() const {
	return _proMode.value();
}

bool Settings::ghostHideRead() const {
	return proMode() && _ghostHideRead.current();
}

void Settings::setGhostHideRead(bool enabled) {
	if (_ghostHideRead.current() != enabled) {
		_ghostHideRead = enabled;
		save();
	}
}

rpl::producer<bool> Settings::ghostHideReadValue() const {
	return _ghostHideRead.value();
}

bool Settings::ghostHideTyping() const {
	return proMode() && _ghostHideTyping.current();
}

void Settings::setGhostHideTyping(bool enabled) {
	if (_ghostHideTyping.current() != enabled) {
		_ghostHideTyping = enabled;
		save();
	}
}

rpl::producer<bool> Settings::ghostHideTypingValue() const {
	return _ghostHideTyping.value();
}

bool Settings::ghostHideOnline() const {
	return proMode() && _ghostHideOnline.current();
}

void Settings::setGhostHideOnline(bool enabled) {
	if (_ghostHideOnline.current() != enabled) {
		_ghostHideOnline = enabled;
		save();
	}
}

rpl::producer<bool> Settings::ghostHideOnlineValue() const {
	return _ghostHideOnline.value();
}

bool Settings::saveViewOnceMedia() const {
	return proMode() && _saveViewOnceMedia.current();
}

void Settings::setSaveViewOnceMedia(bool enabled) {
	if (_saveViewOnceMedia.current() != enabled) {
		_saveViewOnceMedia = enabled;
		save();
	}
}

rpl::producer<bool> Settings::saveViewOnceMediaValue() const {
	return _saveViewOnceMedia.value();
}

bool Settings::antiDeleteMessages() const {
	return proMode() && _antiDeleteMessages.current();
}

void Settings::setAntiDeleteMessages(bool enabled) {
	if (_antiDeleteMessages.current() != enabled) {
		_antiDeleteMessages = enabled;
		save();
	}
}

rpl::producer<bool> Settings::antiDeleteMessagesValue() const {
	return _antiDeleteMessages.value();
}

bool Settings::antiEditMessages() const {
	return proMode() && _antiEditMessages.current();
}

void Settings::setAntiEditMessages(bool enabled) {
	if (_antiEditMessages.current() != enabled) {
		_antiEditMessages = enabled;
		save();
	}
}

rpl::producer<bool> Settings::antiEditMessagesValue() const {
	return _antiEditMessages.value();
}

bool Settings::noForwardTag() const {
	return proMode() && _noForwardTag.current();
}

void Settings::setNoForwardTag(bool enabled) {
	if (_noForwardTag.current() != enabled) {
		_noForwardTag = enabled;
		save();
	}
}

rpl::producer<bool> Settings::noForwardTagValue() const {
	return _noForwardTag.value();
}

bool Settings::confirmBeforeSend() const {
	return proMode() && _confirmBeforeSend.current();
}

void Settings::setConfirmBeforeSend(bool enabled) {
	if (_confirmBeforeSend.current() != enabled) {
		_confirmBeforeSend = enabled;
		save();
	}
}

rpl::producer<bool> Settings::confirmBeforeSendValue() const {
	return _confirmBeforeSend.value();
}

void Settings::load() {
	QFile file(ConfigFilePath());
	if (!file.open(QIODevice::ReadOnly)) {
		return;
	}
	const auto doc = QJsonDocument::fromJson(file.readAll());
	if (!doc.isObject()) {
		return;
	}
	const auto obj = doc.object();
	if (obj.contains("proMode")) _proMode = obj["proMode"].toBool(false);
	if (obj.contains("ghostHideRead")) _ghostHideRead = obj["ghostHideRead"].toBool(true);
	if (obj.contains("ghostHideTyping")) _ghostHideTyping = obj["ghostHideTyping"].toBool(true);
	if (obj.contains("ghostHideOnline")) _ghostHideOnline = obj["ghostHideOnline"].toBool(true);
	if (obj.contains("saveViewOnceMedia")) _saveViewOnceMedia = obj["saveViewOnceMedia"].toBool(true);
	if (obj.contains("antiDeleteMessages")) _antiDeleteMessages = obj["antiDeleteMessages"].toBool(true);
	if (obj.contains("antiEditMessages")) _antiEditMessages = obj["antiEditMessages"].toBool(true);
	if (obj.contains("noForwardTag")) _noForwardTag = obj["noForwardTag"].toBool(false);
	if (obj.contains("confirmBeforeSend")) _confirmBeforeSend = obj["confirmBeforeSend"].toBool(false);
}

void Settings::save() {
	QJsonObject obj;
	obj["proMode"] = _proMode.current();
	obj["ghostHideRead"] = _ghostHideRead.current();
	obj["ghostHideTyping"] = _ghostHideTyping.current();
	obj["ghostHideOnline"] = _ghostHideOnline.current();
	obj["saveViewOnceMedia"] = _saveViewOnceMedia.current();
	obj["antiDeleteMessages"] = _antiDeleteMessages.current();
	obj["antiEditMessages"] = _antiEditMessages.current();
	obj["noForwardTag"] = _noForwardTag.current();
	obj["confirmBeforeSend"] = _confirmBeforeSend.current();

	QDir().mkpath(cWorkingDir() + u"tdata"_q);
	QFile file(ConfigFilePath());
	if (file.open(QIODevice::WriteOnly)) {
		file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
	}
}

} // namespace Matzu
