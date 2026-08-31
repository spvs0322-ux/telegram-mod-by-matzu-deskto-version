/*
This file is part of Telegram Desktop Matzu Mod.
*/
#pragma once

#include <rpl/variable.h>
#include <QtCore/QString>

namespace Matzu {

class Settings final {
public:
	static Settings &Instance();

	// Master Toggle
	[[nodiscard]] bool proMode() const;
	void setProMode(bool enabled);
	[[nodiscard]] rpl::producer<bool> proModeValue() const;

	// Ghost Mode
	[[nodiscard]] bool ghostHideRead() const;
	void setGhostHideRead(bool enabled);
	[[nodiscard]] rpl::producer<bool> ghostHideReadValue() const;

	[[nodiscard]] bool ghostHideTyping() const;
	void setGhostHideTyping(bool enabled);
	[[nodiscard]] rpl::producer<bool> ghostHideTypingValue() const;

	[[nodiscard]] bool ghostHideOnline() const;
	void setGhostHideOnline(bool enabled);
	[[nodiscard]] rpl::producer<bool> ghostHideOnlineValue() const;

	// View-Once Silent Download
	[[nodiscard]] bool saveViewOnceMedia() const;
	void setSaveViewOnceMedia(bool enabled);
	[[nodiscard]] rpl::producer<bool> saveViewOnceMediaValue() const;

	// Anti-Delete & Anti-Edit
	[[nodiscard]] bool antiDeleteMessages() const;
	void setAntiDeleteMessages(bool enabled);
	[[nodiscard]] rpl::producer<bool> antiDeleteMessagesValue() const;

	[[nodiscard]] bool antiEditMessages() const;
	void setAntiEditMessages(bool enabled);
	[[nodiscard]] rpl::producer<bool> antiEditMessagesValue() const;

	// Forwards
	[[nodiscard]] bool noForwardTag() const;
	void setNoForwardTag(bool enabled);
	[[nodiscard]] rpl::producer<bool> noForwardTagValue() const;

	// Confirm Dialogs
	[[nodiscard]] bool confirmBeforeSend() const;
	void setConfirmBeforeSend(bool enabled);
	[[nodiscard]] rpl::producer<bool> confirmBeforeSendValue() const;

	void load();
	void save();

private:
	Settings();
	~Settings() = default;

	rpl::variable<bool> _proMode = false;
	rpl::variable<bool> _ghostHideRead = true;
	rpl::variable<bool> _ghostHideTyping = true;
	rpl::variable<bool> _ghostHideOnline = true;
	rpl::variable<bool> _saveViewOnceMedia = true;
	rpl::variable<bool> _antiDeleteMessages = true;
	rpl::variable<bool> _antiEditMessages = true;
	rpl::variable<bool> _noForwardTag = false;
	rpl::variable<bool> _confirmBeforeSend = false;
};

[[nodiscard]] inline Settings &Config() {
	return Settings::Instance();
}

} // namespace Matzu
