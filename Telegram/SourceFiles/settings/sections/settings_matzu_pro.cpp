/*
This file is part of Telegram Desktop Matzu Mod.
*/
#include "settings/sections/settings_matzu_pro.h"

#include "core/matzu_settings.h"
#include "lang/lang_keys.h"
#include "settings/sections/settings_main.h"
#include "settings/settings_builder.h"
#include "settings/settings_common.h"
#include "settings/settings_common_session.h"
#include "ui/vertical_list.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/checkbox.h"
#include "ui/widgets/labels.h"
#include "ui/wrap/vertical_layout.h"
#include "ui/wrap/slide_wrap.h"
#include "window/window_session_controller.h"
#include "styles/style_menu_icons.h"
#include "styles/style_settings.h"

namespace Settings {
namespace {

using namespace Builder;

void BuildMatzuProSectionContent(SectionBuilder &builder) {
	builder.addSubsectionTitle(rpl::single(QString("Master Switch")));

	// Master Toggle Switch
	const auto master = builder.addCheckbox({
		.id = u"matzu/master"_q,
		.title = rpl::single(QString("Enable PRO Mode by Matzu")),
		.checked = Matzu::Config().proMode(),
	});
	if (master) {
		master->checkedChanges(
		) | rpl::start_with_next([](bool checked) {
			Matzu::Config().setProMode(checked);
		}, master->lifetime());
	}

	builder.addDividerText(rpl::single(
		QString("Turn ON to activate all Ghost Mode, Silent View-Once Media Download, and Anti-Delete features.")
	));

	// Scope visible only when Master switch is ON
	builder.scope([&] {
		builder.addSkip();
		builder.addSubsectionTitle(rpl::single(QString("👻 Privacy & Ghost Mode")));

		// Ghost Hide Read
		const auto ghostRead = builder.addCheckbox({
			.id = u"matzu/ghost_read"_q,
			.title = rpl::single(QString("Hide Read / Seen (Ghost Mode)")),
			.checked = Matzu::Config().ghostHideRead(),
		});
		if (ghostRead) {
			ghostRead->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setGhostHideRead(checked);
			}, ghostRead->lifetime());
		}

		// Hide Typing
		const auto ghostTyping = builder.addCheckbox({
			.id = u"matzu/ghost_typing"_q,
			.title = rpl::single(QString("Hide Typing & Recording Status")),
			.checked = Matzu::Config().ghostHideTyping(),
		});
		if (ghostTyping) {
			ghostTyping->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setGhostHideTyping(checked);
			}, ghostTyping->lifetime());
		}

		// Hide Online Status
		const auto ghostOnline = builder.addCheckbox({
			.id = u"matzu/ghost_online"_q,
			.title = rpl::single(QString("Hide Online Status")),
			.checked = Matzu::Config().ghostHideOnline(),
		});
		if (ghostOnline) {
			ghostOnline->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setGhostHideOnline(checked);
			}, ghostOnline->lifetime());
		}

		// Anti-Delete
		const auto antiDelete = builder.addCheckbox({
			.id = u"matzu/anti_delete"_q,
			.title = rpl::single(QString("Anti-Delete Messages (Keep Deleted)")),
			.checked = Matzu::Config().antiDeleteMessages(),
		});
		if (antiDelete) {
			antiDelete->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setAntiDeleteMessages(checked);
			}, antiDelete->lifetime());
		}

		// Anti-Edit
		const auto antiEdit = builder.addCheckbox({
			.id = u"matzu/anti_edit"_q,
			.title = rpl::single(QString("Anti-Edit Messages (Save Edit History)")),
			.checked = Matzu::Config().antiEditMessages(),
		});
		if (antiEdit) {
			antiEdit->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setAntiEditMessages(checked);
			}, antiEdit->lifetime());
		}

		builder.addDivider();
		builder.addSkip();
		builder.addSubsectionTitle(rpl::single(QString("🎬 Media & Security Tweaks")));

		// Save View-Once Media
		const auto saveViewOnce = builder.addCheckbox({
			.id = u"matzu/save_view_once"_q,
			.title = rpl::single(QString("Save 1-View (View-Once) Media Silently")),
			.checked = Matzu::Config().saveViewOnceMedia(),
		});
		if (saveViewOnce) {
			saveViewOnce->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setSaveViewOnceMedia(checked);
			}, saveViewOnce->lifetime());
		}

		// No Forward Tag
		const auto noForward = builder.addCheckbox({
			.id = u"matzu/no_forward_tag"_q,
			.title = rpl::single(QString("Forward Without Sender Tag")),
			.checked = Matzu::Config().noForwardTag(),
		});
		if (noForward) {
			noForward->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setNoForwardTag(checked);
			}, noForward->lifetime());
		}

		builder.addDivider();
		builder.addSkip();
		builder.addSubsectionTitle(rpl::single(QString("⚡ Productivity & Limits")));

		// Confirm Before Send
		const auto confirmSend = builder.addCheckbox({
			.id = u"matzu/confirm_send"_q,
			.title = rpl::single(QString("Confirm Before Sending Voice & Calls")),
			.checked = Matzu::Config().confirmBeforeSend(),
		});
		if (confirmSend) {
			confirmSend->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setConfirmBeforeSend(checked);
			}, confirmSend->lifetime());
		}

		builder.addDividerText(rpl::single(
			QString("Unlimited Multi-Account Support (up to 100 accounts) is permanently unlocked.")
		));
	}, Matzu::Config().proModeValue());
}

class MatzuPro : public Section<MatzuPro> {
public:
	MatzuPro(
		QWidget *parent,
		not_null<Window::SessionController*> controller);

	[[nodiscard]] rpl::producer<QString> title() override;

private:
	void setupContent();
};

const auto kMeta = BuildHelper({
	.id = MatzuPro::Id(),
	.parentId = MainId(),
	.title = &tr::lng_settings_section_privacy,
	.icon = &st::menuIconManage,
}, [](SectionBuilder &builder) {
	BuildMatzuProSectionContent(builder);
});

const SectionBuildMethod kMatzuProSection = kMeta.build;

MatzuPro::MatzuPro(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section<MatzuPro>(parent, controller) {
	setupContent();
}

rpl::producer<QString> MatzuPro::title() {
	return rpl::single(QString("PRO Mode by Matzu"));
}

void MatzuPro::setupContent() {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
	build(content, kMatzuProSection);
	Ui::ResizeFitChild(this, content);
}

} // namespace

Type MatzuProId() {
	return MatzuPro::Id();
}

} // namespace Settings
