export { default as default } from "./addon";

export enum ServiceType {
	/**
	 * The service is a device driver.
	 */
	KERNEL_DRIVER = 0x1,

	/**
	 * The service is a file system driver.
	 */
	FILE_SYSTEM_DRIVER = 0x2,

	/**
	 * The service runs in its own process.
	 */
	WIN32_OWN_PROCESS = 0x10,

	/**
	 * The service shares a process with other services.
	 */
	WIN32_SHARE_PROCESS = 0x20,

	/**
	 * The service runs in its own process under the logged-on user account.
	 */
	USER_OWN_PROCESS = 0x50,

	/**
	 * The service shares a process with one or more other services that run under the logged-on user account.
	 */
	USER_SHARE_PROCESS = 0x60,

	/**
	 * The service can interact with the desktop.
	 */
	INTERACTIVE_PROCESS = 0x100
}

export enum ServiceState {
	STOPPED = 0x1,
	START_PENDING = 0x2,
	STOP_PENDING = 0x3,
	RUNNING = 0x4,
	CONTINUE_PENDING = 0x5,
	PAUSE_PENDING = 0x6,
	PAUSED = 0x7,
}

export enum ServiceControls {
	ACCEPT_STOP = 0x1,
	PAUSE_CONTINUE = 0x2,
	ACCEPT_SHUTDOWN = 0x4,
	PARAMCHANGE = 0x8,
	NETBINDCHANGE = 0x10,
	HARDWAREPROFILECHANGE = 0x20,
	POWEREVENT = 0x40,
	SESSIONCHANGE = 0x80,
	PRESHUTDOWN = 0x100,
}